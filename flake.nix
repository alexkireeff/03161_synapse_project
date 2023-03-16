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
          arduino
        ];

        shellHook = ''
          sudo sysctl -w kernel.unprivileged_userns_clone=1
          zsh
          sudo sysctl -w kernel.unprivileged_userns_clone=0
          exit'';
      };
  };
}
