{ cmake, stdenv }:

stdenv.mkDerivation {
  name = "pprintpp";
  src = ./.;

  nativeBuildInputs = [ cmake ];

  doCheck = true;

  meta = with stdenv.lib; {
    description = "Typesafe Python style printf for C++";
    homepage = "https://github.com/tfc/pprintpp";
    license = licenses.mit;
    platforms = platforms.linux ++ platforms.darwin;
  };
}
