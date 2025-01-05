{
  description = "Zig Devshell";

  inputs.devshell.url = "github:numtide/devshell";
  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.zig.url = "github:mitchellh/zig-overlay";
	inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  inputs.flake-compat = {
    url = "github:edolstra/flake-compat";
    flake = false;
  };

  outputs = {
    flake-utils,
    devshell,
    nixpkgs,
    zig,
    ...
  }:
    flake-utils.lib.eachDefaultSystem (system: {
      devShells.default = let
        pkgs = import nixpkgs {
          inherit system;

          overlays = [devshell.overlays.default zig.overlays.default];
        };
      in
        pkgs.mkShell {
          packages = with pkgs; [
            zigpkgs.default
            zls
          ];
					
					buildInputs = with pkgs; [
                clang-tools
                cmake
                codespell
                conan
                cppcheck
                doxygen
                gtest
                lcov
                vcpkg
                vcpkg-tool
                glfw
								glfw-wayland
                glfw3
								glm
								assimp
								# libdecor
								mesa
								alsa-lib
								dbus
								fontconfig
                glew
								libGL
								libpulseaudio
								libxkbcommon
								makeWrapper
                SDL2
                SDL2_image
								patchelf
								speechd
								udev
								# xwayland
								xorg.libX11
								xorg.libXcursor
								xorg.libXext
								xorg.libXfixes
								xorg.libXi
								xorg.libXinerama
								xorg.libXrandr
								xorg.libXrender
                vulkan-loader
                vulkan-tools
                vulkan-headers
					];
					# DISPLAY=":1";

					# shellHook = '' export LD_LIBRARY_PATH=${pkgs.wayland}/lib:$LD_LIBRARY_PATH '';
					shellHook = ''
					export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:${
						with pkgs;
						pkgs.lib.makeLibraryPath [ libGL xorg.libX11 xorg.libXi libxkbcommon mesa xorg.libXrender xorg.libXinerama glfw-wayland]
					}"
					'';

        };
			});
	}
