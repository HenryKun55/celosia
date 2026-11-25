/* Round P2: import stub of the CelosiaK2P2 syscall library (celosia_k2.skprx; NIDs fixed in exports.yml), in the format
 * vita-libs-gen-2 writes. Only the P2 bridge celosia_p2u.suprx links it: an older resident K2 instance (without the
 * library) makes that bridge's load fail, and the game runs without sampling. */
.arch armv7a

.section .vitalink.fstubs.CelosiaK2P2,"ax",%progbits

	.align 4
	.global celosiaK2P2
	.type celosiaK2P2, %function
celosiaK2P2:
	.word 0x00010000
	.word 0xC2C2C2F2
	.word 0xC2F2C001
	.align 4
