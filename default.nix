{ nixpkgs ? <nixpkgs>
, pkgs ? import nixpkgs { overlays = [ (import ./overlay.nix) ]; }
}:
let
  inherit (import ./overlay.nix pkgs pkgs) pprintpp;
in
{
  inherit pprintpp;

  compiled-with = pkgs.recurseIntoAttrs {
    clang = pprintpp.override { stdenv = pkgs.clangStdenv; };
    gcc = pprintpp.override { stdenv = pkgs.gccStdenv; };
  };
}
