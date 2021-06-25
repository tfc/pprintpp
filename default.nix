{ nixpkgs ? <nixpkgs>
, pkgs ? import nixpkgs { overlays = [ (import ./overlay.nix) ]; }
}:
let
  inherit (import ./overlay.nix pkgs pkgs) pprintpp;
in
rec {
  inherit pprintpp;

  compiled-with = pkgs.recurseIntoAttrs {
    clang = pprintpp.override { stdenv = pkgs.clangStdenv; };
    gcc = pprintpp.override { stdenv = pkgs.gccStdenv; };
  };

  # https://nixos.wiki/wiki/Nixpkgs/Building_RPM_DEB_with_nixpkgs uses a different
  # method but it seems to require a more classical debian-specific description.
  # running cpack with the right cmake config does also work, not sure if this
  # good:
  deb = pkgs.vmTools.runInLinuxImage (pprintpp.overrideAttrs (old: {
    name = "pprintpp.deb";
    diskImage = pkgs.vmTools.diskImageFuns.ubuntu2004x86_64 {
      extraPackages = [ "cmake" ];
    };
    configurePhase = ''
      cmake "$src"
    '';
    installPhase = ''
      cpack -G DEB .
      ls -lsa
      mv pprintpp-*.deb "$out"
    '';
  }));
}
