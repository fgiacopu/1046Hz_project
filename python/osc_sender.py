# osc_sender.py

from pythonosc.udp_client import SimpleUDPClient

class OscSender:
    def __init__(self, ip="127.0.0.1", port=9000, debug=False, debug_filter=None):
        """
        ip: OSC destination (localhost for tests)
        port: UDP port
        debug: if True prints the sent messages
        """
        self.client = SimpleUDPClient(ip, port)
        self.debug = debug
        self.debug_filter = debug_filter

    def send(self, address, value):
        """
        Sends a single float/int value
        """
        try:
            self.client.send_message(address, value)
           
            if self.debug_filter is None:
                print(f"[OSC] {address} -> {value}")
            else:
                if any(f in address for f in self.debug_filter):
                    print(f"[OSC] {address} -> {value}")

        except Exception as e:
            print(f"[OSC ERROR] {e}")

    def send_bundle(self, address, values):
        """
        Sends multiple values under the same address
        (useful if in the future you want to send vectors)
        """
        try:
            self.client.send_message(address, values)
            
            if self.debug:
                if self.debug_filter is None:
                    print(f"[OSC] {address} -> {values}")
                else:
                    if any(f in address for f in self.debug_filter):
                        print(f"[OSC] {address} -> {values}")

        except Exception as e:
            print(f"[OSC ERROR] {e}")
