{
  inputs = {
    nixpkgs.url = github:NixOS/nixpkgs/master;
    flake-utils.url = github:numtide/flake-utils;
  };
  outputs = { self, nixpkgs, flake-utils }:
    (flake-utils.lib.eachSystem [ flake-utils.lib.system.x86_64-linux ] (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        packages.pprintpp = pkgs.callPackage ./nix/build.nix { };
        packages.default = self.packages.${system}.pprintpp;
        checks = {
          clang-build = self.packages.${system}.pprintpp.override {
            stdenv = pkgs.clangStdenv;
          };
          gcc-build = self.packages.${system}.pprintpp.override {
            stdenv = pkgs.clangStdenv;
          };
          example-standalone = pkgs.stdenv.mkDerivation {
            name = "pprintpp-example-project";
            src = ./example;
            buildInputs = [ self.packages.${system}.pprintpp ];
            nativeBuildInputs = [ pkgs.cmake ];
          };
        };
      }
    )) // {
      overlays.default = final: prev: {
        pprintpp = final.callPackage ./nix/build.nix { };
      };
    };
}
