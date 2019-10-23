{
  nixpkgs ? <nixpkgs>,
  pkgs ? import nixpkgs {}
}:

pkgs.stdenv.mkDerivation {
  name = "pprintpp";
  src = ./.;
  nativeBuildInputs = [ pkgs.cmake ];
}
