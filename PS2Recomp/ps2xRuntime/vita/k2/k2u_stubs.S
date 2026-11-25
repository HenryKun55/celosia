/* Round K2: import stubs of the CelosiaK2 syscall library (celosia_k2.skprx; NIDs fixed in exports.yml), in the format
 * vita-libs-gen-2 writes. Only the bridge celosia_k2u.suprx links them: the eboot cannot import a library that is
 * loaded after it started. */
.arch armv7a

.section .vitalink.fstubs.CelosiaK2,"ax",%progbits

	.align 4
	.global celosiaK2Hello
	.type celosiaK2Hello, %function
celosiaK2Hello:
	.word 0x00010000
	.word 0xC2C2C201
	.word 0xC2C20001
	.align 4

	.align 4
	.global celosiaK2PmuArm
	.type celosiaK2PmuArm, %function
celosiaK2PmuArm:
	.word 0x00010000
	.word 0xC2C2C201
	.word 0xC2C20002
	.align 4

	.align 4
	.global celosiaK2PmuRead
	.type celosiaK2PmuRead, %function
celosiaK2PmuRead:
	.word 0x00010000
	.word 0xC2C2C201
	.word 0xC2C20003
	.align 4

	.align 4
	.global celosiaK2Walk
	.type celosiaK2Walk, %function
celosiaK2Walk:
	.word 0x00010000
	.word 0xC2C2C201
	.word 0xC2C20004
	.align 4

	.align 4
	.global celosiaK2Remap
	.type celosiaK2Remap, %function
celosiaK2Remap:
	.word 0x00010000
	.word 0xC2C2C201
	.word 0xC2C20005
	.align 4

	.align 4
	.global celosiaK2Restore
	.type celosiaK2Restore, %function
celosiaK2Restore:
	.word 0x00010000
	.word 0xC2C2C201
	.word 0xC2C20006
	.align 4
