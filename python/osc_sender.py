# osc_sender.py

from pythonosc.udp_client import SimpleUDPClient

class OscSender:
    def __init__(self, ip="127.0.0.1", port=9000, debug=False):
        """
        ip: OSC destination (localhost for tests)
        port: UDP port
        debug: if True prints the sent messages
        """
        self.client = SimpleUDPClient(ip, port)
        self.debug = debug

    def send(self, address, value):
        """
        Sends a single float/int value
        """
        try:
            self.client.send_message(address, value)
            if self.debug:
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
                print(f"[OSC] {address} -> {values}")
        except Exception as e:
            print(f"[OSC ERROR] {e}")
