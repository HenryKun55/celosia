/* Round A3: import stub of the CelosiaK2A3 syscall library (celosia_k2.skprx; NIDs fixed in exports.yml), in the format
 * vita-libs-gen-2 writes. Only the A3 bridge celosia_a3u.suprx links it: an older resident K2 instance (without the
 * library) makes that bridge's load fail, and the game stays at 444 MHz. */
.arch armv7a

.section .vitalink.fstubs.CelosiaK2A3,"ax",%progbits

	.align 4
	.global celosiaK2Clock
	.type celosiaK2Clock, %function
celosiaK2Clock:
	.word 0x00010000
	.word 0xC2C2C2A3
	.word 0xC2A3C001
	.align 4
