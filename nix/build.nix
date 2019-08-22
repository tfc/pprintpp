{
  stdenv,
  cmake,
  nix-gitignore
}:

stdenv.mkDerivation {
  name = "pprintpp";
  nativeBuildInputs = [ cmake ];
  src = nix-gitignore.gitignoreSourcePure ["nix\n" ../.gitignore]  ./..;
}
