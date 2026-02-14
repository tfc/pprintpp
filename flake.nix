{
  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    flake-parts.inputs.nixpkgs-lib.follows = "nixpkgs";

    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    treefmt-nix.url = "github:numtide/treefmt-nix";
    treefmt-nix.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "x86_64-linux"
        "x86_64-darwin"
        "aarch64-linux"
        "aarch64-darwin"
      ];
      imports = [
        inputs.treefmt-nix.flakeModule
      ];
      perSystem =
        {
          config,
          pkgs,
          system,
          ...
        }:
        {
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
          };

          treefmt = {
            projectRootFile = "flake.lock";
            programs = {
              deadnix.enable = true;
              nixfmt.enable = true;
              shfmt.enable = true;
              statix.enable = true;
              clang-format.enable = true;
              cmake-format.enable = true;
            };
            settings.formatter.cmake-format.excludes = [ "pkg-config.pc.cmake" ];
          };
        };
      flake = {
        overlays.default = import ./overlay.nix;
      };
    };
}
