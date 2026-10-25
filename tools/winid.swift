// tools/winid.swift <pid> (tools/vita3k.sh SHOTS=...): the CGWindowID of that process's largest on-screen window (for screencapture -l).
import CoreGraphics
import Foundation
let pid = Int(CommandLine.arguments[1])!
let list = CGWindowListCopyWindowInfo([.optionOnScreenOnly], kCGNullWindowID) as! [[String: Any]]
// Vita3K's game window is named after the app ("... | Shadow of the Colossus | ..."
// with its title id); the library window is not: that one wins, else the largest.
var best: (Int, Double) = (0, 0)
for w in list where (w[kCGWindowOwnerPID as String] as? Int) == pid {
    let b = w[kCGWindowBounds as String] as! [String: Double]
    let name = w[kCGWindowName as String] as? String ?? ""
    let game = name.contains("CELO") || name.contains("|") || name.contains("FPS")
    let area = (b["Width"] ?? 0) * (b["Height"] ?? 0) + (game ? 1e12 : 0)
    if CommandLine.arguments.count > 2 { print(w[kCGWindowNumber as String] ?? 0, name, b) }
    if area > best.1 { best = (w[kCGWindowNumber as String] as! Int, area) }
}
print(best.0)
