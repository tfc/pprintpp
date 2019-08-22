{
  nixpkgs ? <nixpkgs>,
  pkgs ? import nixpkgs { overlays = [ (import ./vm.nix) ]; }
}:

let
  pprintpp = pkgs.callPackage (import ./build.nix) {};
  packageDerivation = generatorName:
    pprintpp.overrideAttrs (old: {
      inherit generatorName;
      nativeBuildInputs = old.nativeBuildInputs ++ [ pkgs.rpm ];
      configurePhase = "cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_BUILD_TYPE=Release $src";
      installPhase = ''
        ext=''${generatorName,,}
        echo "extension is $ext"

        cpack -G $generatorName
        mkdir -p $out/nix-support
        cp *.$ext $out/
        for file in $(find $out -name "*.$ext"); do
          echo "file $ext $file" >> $out/nix-support/hydra-build-products
        done
      '';
    });
  packageGenerator = generator: diskImage:
    pkgs.vmTools.runInLinuxImage (packageDerivation generator // {
      inherit diskImage;
    });
  deb = packageGenerator "DEB" pkgs.vmTools.diskImages.ubuntu1904x86_64;
  rpm = packageGenerator "RPM" pkgs.vmTools.diskImages.fedora30x86_64;

in {
  inherit pprintpp;
  packages = pkgs.runCommand "gather-packages" {} ''
    mkdir -p $out/nix-support
    for folder in ${deb} ${rpm}; do
      cat $folder/nix-support/hydra-build-products >> $out/nix-support/hydra-build-products
    done
  '';
}
