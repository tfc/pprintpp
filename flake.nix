{
  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    flake-parts.inputs.nixpkgs-lib.follows = "nixpkgs";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    pre-commit-hooks.url = "github:cachix/pre-commit-hooks.nix";
  };

  outputs = inputs: inputs.flake-parts.lib.mkFlake { inherit inputs; }
    {
      systems = [ "x86_64-linux" ];
      perSystem = { config, pkgs, system, ... }: {
        packages.pprintpp = pkgs.callPackage ./nix/build.nix { };
        packages.default = config.packages.pprintpp;

        devShells.default = pkgs.mkShell {
          inputsFrom = [ config.packages.pprintpp ];
          shellHook = ''
            ${config.checks.pre-commit-check.shellHook}
          '';
        };
        checks = {
          clang-build = config.packages.pprintpp.override {
            stdenv = pkgs.clangStdenv;
          };
          gcc-build = config.packages.pprintpp.override {
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
    } // {
    overlays.default = final: prev: {
      pprintpp = final.callPackage ./nix/build.nix { };
    };
  };
}
