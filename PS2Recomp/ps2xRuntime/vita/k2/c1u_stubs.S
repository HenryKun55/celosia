/* Round C1: import stub of the CelosiaK2C1 syscall library (celosia_k2.skprx; NIDs fixed in exports.yml), in the format
 * vita-libs-gen-2 writes. Only the C1 bridge celosia_c1u.suprx links it: an older resident K2 instance (without the
 * library) makes that bridge's load fail, and the game stays on cores 0-2. */
.arch armv7a

.section .vitalink.fstubs.CelosiaK2C1,"ax",%progbits

	.align 4
	.global celosiaK2Core3
	.type celosiaK2Core3, %function
celosiaK2Core3:
	.word 0x00010000
	.word 0xC2C2C2C1
	.word 0xC2C1C001
	.align 4
