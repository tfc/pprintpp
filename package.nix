{ cmake, stdenv, lib }:

stdenv.mkDerivation {
  name = "pprintpp";

  src = lib.sourceByRegex ./. [
    "^include.*"
    "^test.*"
    "^example.*"
    "CMakeLists.txt"
    "pkg-config.pc.cmake"
  ];

  nativeBuildInputs = [ cmake ];

  doCheck = true;

  meta = with lib; {
    description = "Typesafe Python style printf for C++";
    homepage = "https://github.com/tfc/pprintpp";
    license = licenses.mit;
    platforms = platforms.linux ++ platforms.darwin;
  };
}
