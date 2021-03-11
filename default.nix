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

  # the debian and ubuntu images here are very old.
  # their definitions come from <nixpkgs/build-support/vm/default.nix>
  # and can be easily defined for newer distro versions of both fedora/ubuntu.
  # TODO: Do this, and also upstream the changes to nixpkgs!
  deb = pkgs.vmTools.runInLinuxImage (pkgs.stdenv.mkDerivation {
    name = "pprintpp-debian";
    src = ./.;
    installPhase = ''
      cpack -G DEB
      install -m 755 *.deb $out/
    '';
    memSize = 512;
    diskImage = pkgs.vmTools.diskImageFuns.ubuntu1804x86_64 {
      extraPackages = ["cmake"];
    };
  });
  rpm = pkgs.vmTools.runInLinuxImage (pkgs.stdenv.mkDerivation {
    name = "pprintpp-debian";
    src = ./.;
    installPhase = ''
      cpack -G RPM
      install -m 755 *.rpm $out/
    '';
    memSize = 512;
    diskImage = pkgs.vmTools.diskImageFuns.fedora27x86_64 {
      extraPackages = ["cmake"];
    };
  });
}
