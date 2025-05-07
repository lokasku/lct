# SPDX-License-Identifier: ISC
# SPDX-FileCopyrightText: Copyright 2025 lokasku <lukasku@proton.me>

let
  pkgs = import <nixpkgs> {};
in
  pkgs.mkShell {
    packages = with pkgs; [
      gcc
      gnumake
    ];
  }
