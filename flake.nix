{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/master";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };
  outputs = { self, nixpkgs, flake-parts }:
    flake-parts.lib.mkFlake { inherit self; }
      {
        systems = [ "x86_64-linux" ];
        perSystem = { config, pkgs, system, ... }: {
          packages.pprintpp = pkgs.callPackage ./nix/build.nix { };
          packages.default = config.packages.pprintpp;
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
          };
        };
      } // {
      overlays.default = final: prev: {
        pprintpp = final.callPackage ./nix/build.nix { };
      };
    };
}
