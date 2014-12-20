Walltech-OLED-Watch-v4.3
========================

This is the source code for the latest prototype of my Arduino OLED wristwatch. v4.3 now has a MCP9808 temperature sensor and interrupt connections to enable deep sleep mode which could allow >2 months on battery! The program is 1k oversized and needs compressing somehow. This will be the non-interrupt version that will have a 2 day battery life like previous versions. When the "temperature" face is selected, the icons shows, the 1 second delay passes, and the sketch either resets, or hangs completely. I assume it's a memory issue from experience and forum browsing.

Required libraries are in the libraries folder. To use them, drop the folder's contents into your Arduino IDE's "LIBRARIES" folder in documents. If there isn't one, create one.
