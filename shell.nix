{ pkgs ? import <nixpkgs> {} }:

let
  libs = [
    pkgs.gcc.cc.lib
    pkgs.libpng.dev
  ];
in
pkgs.mkShell {
  buildInputs = [

  ] ++ libs;

  shellHook = ''
    for lib in ${pkgs.lib.concatStringsSep " " (map (p: "${p}/lib") libs)};
    do
      export LD_LIBRARY_PATH="$lib:$LD_LIBRARY_PATH"
    done

  '';
}
