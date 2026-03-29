{ pkgs, llvmPackages, ... }:

llvmPackages.stdenv.mkDerivation {
  pname = "redis-server";
  version = "0.1";

  src = ../.;

  dontUnpack = true;

  nativeBuildInputs = (with pkgs; [
    # LLVM toolchain
    # cmake
    llvmPackages.llvm
    llvmPackages.clang-tools
    pkg-config
  ]);

  # buildPhase = ''
  #   cc main.c -o redis-server
  # '';

  installPhase = ''
    mkdir -p $out/bin
    cp redis-server $out/bin -r
  '';

  # buildInputs = [ imlib2 ];
}
