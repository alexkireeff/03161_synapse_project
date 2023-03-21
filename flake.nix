{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs";
  outputs = {
    self,
    nixpkgs,
  }: {
    devShells.x86_64-linux.default = with import nixpkgs {
      system = "x86_64-linux";
      config = {
        allowUnfree = true;
        cudaSupport = true;
      };
    };
      mkShell {
        buildInputs = with pkgs; [
          arduino-cli
          gnumake
          screen
        ];

        shellHook = ''
          zsh
          exit'';
      };
  };
}
