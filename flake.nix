{
  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    flake-parts.inputs.nixpkgs-lib.follows = "nixpkgs";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    pre-commit-hooks.url = "github:cachix/pre-commit-hooks.nix";
  };

  outputs = inputs: inputs.flake-parts.lib.mkFlake { inherit inputs; } {
    systems = [ "x86_64-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin" ];
    perSystem = { config, self', pkgs, system, ... }: {
      _module.args.pkgs = import inputs.nixpkgs {
        inherit system;
        overlays = [ inputs.self.overlays.default ];
      };
      packages = {
        inherit (pkgs) pprintpp;
        default = config.packages.pprintpp;
      };

      devShells.default = pkgs.mkShell {
        inputsFrom = [ config.packages.default ];
        inherit (config.checks.pre-commit-check) shellHook;
      };
      checks = {
        clang-build = pkgs.pprintpp.override {
          stdenv = pkgs.clangStdenv;
        };
        gcc-build = pkgs.pprintpp.override {
          stdenv = pkgs.clangStdenv;
        };
        example-standalone = pkgs.stdenv.mkDerivation {
          name = "pprintpp-example-project";
          src = ./example;
          buildInputs = [ config.packages.pprintpp ];
          nativeBuildInputs = [ pkgs.cmake ];
        };
        pre-commit-check = inputs.pre-commit-hooks.lib.${system}.run {
          src = ./.;
          hooks = {
            clang-format.enable = true;
            nixpkgs-fmt.enable = true;
            statix.enable = true;
          };
        };
      };
    };
    flake = {
      overlays.default = import ./overlay.nix;
    };
  };
}
