# PAL linking tools

The linker in this directory and its ISO9660/XFF helpers are adapted from
[LightVelox/sotc-vibe-pc](https://github.com/LightVelox/sotc-vibe-pc), commit
`9b9a04c05af1a79e0bccec116f56fc75a82d5383`. The source project and Celosia
are both distributed under GPL-3.0.

Local changes are limited to integration with Celosia's build layout and the
generation of `sotc_dynamic_immediates.txt`, the input format used by
Celosia's PS2Recomp fork for XFF relocation sites. The related
`auto_stub_by_name` configuration switch in PS2Recomp is also adapted from
that revision; it defaults to `true` to preserve existing Celosia builds.
The PAL boot-time vector syscalls and `sceGetstat` file-status binding likewise
follow that revision while using Celosia's existing VFS implementation.

The tools contain no game data. They read `SCES_533.26`, `KERNEL.XFF`,
`MANAGER.XFF`, and `GAMECORE.XFF` from a user-supplied PAL v1.00 disc and
write all linked/generated artifacts under the ignored `build/` directory.
