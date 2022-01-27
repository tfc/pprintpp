let
  sources = import ./nix/sources.nix;
  pkgs = import sources.nixpkgs {};

  pprintpp = pkgs.callPackage ./nix/build.nix {};
in
{
  inherit pprintpp;

  compiled-with = pkgs.recurseIntoAttrs {
    clang = pprintpp.override { stdenv = pkgs.clangStdenv; };
    gcc = pprintpp.override { stdenv = pkgs.gccStdenv; };
  };
}
