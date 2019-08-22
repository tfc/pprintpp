{
  nixpkgs ? <nixpkgs>,
  pkgs ? import nixpkgs { overlays = [ (import ./vm.nix) ]; }
}:

let
  pprintpp = pkgs.callPackage (import ./build.nix) {};
  packageDerivation = generator:
    let
      ending = if generator == "DEB"
                 then "deb"
                 else if generator == "RPM"
                   then "rpm"
                   else builtins.throw "unknown generator: ${generator}";
      packages = with pkgs; if generator == "RPM" then [ rpm ] else [];
    in pprintpp.overrideAttrs (old: {
      nativeBuildInputs = old.nativeBuildInputs ++ packages;
      configurePhase = "cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_BUILD_TYPE=Release $src";
      installPhase = ''
        cpack -G ${generator}
        mkdir -p $out/nix-support
        cp *.${ending} $out/
        for file in $(find $out -name "*.${ending}"); do
          echo "file ${ending} $file" >> $out/nix-support/hydra-build-products
        done
      '';
    });
  packageGenerator = generator: diskImage:
    pkgs.vmTools.runInLinuxImage (packageDerivation "DEB" // {
      inherit diskImage;
    });

in {
  inherit pprintpp;
  pprintpp-deb = packageGenerator "DEB" pkgs.vmTools.diskImages.ubuntu1904x86_64;
  pprintpp-rpm = packageGenerator "RPM" pkgs.vmTools.diskImages.fedora30x86_64;
}
