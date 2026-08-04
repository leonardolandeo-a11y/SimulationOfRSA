import sys
import os
import json
import subprocess
from PySide6.QtCore import Qt
from PySide6.QtGui import QFont
from PySide6.QtWidgets import (
    QApplication,
    QMainWindow,
    QWidget,
    QVBoxLayout,
    QHBoxLayout,
    QGridLayout,
    QLabel,
    QPushButton,
    QTextEdit,
    QLineEdit,
    QFrame,
    QMessageBox,
)

# Relative executable path for Linux
CLI_PATH = "./main"


class RSAInterface(QMainWindow):

    def __init__(self):
        super().__init__()

        self.setWindowTitle("RSA Cryptography Simulator")
        self.setMinimumSize(900, 850)

        # Storage for key parameters
        self.p_val = ""
        self.q_val = ""
        self.e_val = ""
        self.d_val = ""
        self.n_val = ""

        self.create_interface()

    def run_cpp_backend(self, args):
        """Helper function to execute the compiled C++ executable and parse JSON output."""
        if not os.path.exists(CLI_PATH):
            QMessageBox.critical(
                self,
                "Backend Missing",
                f"Could not find binary executable at '{CLI_PATH}'.\n"
                "Please compile your C++ files first using:\n"
                "g++ -O3 SelectionNumbers.cpp RSA_Functions.cpp RSA_Simulation.cpp -o main"
            )
            return None

        try:
            result = subprocess.run(
                [CLI_PATH] + args,
                capture_output=True,
                text=True,
                check=True
            )
            return json.loads(result.stdout.strip())
        except subprocess.CalledProcessError as e:
            QMessageBox.critical(self, "C++ Error", f"Execution error: {e.stderr}")
            return None
        except json.JSONDecodeError:
            QMessageBox.critical(self, "Parse Error", "Invalid JSON output received from C++ backend.")
            return None

    def create_interface(self):

        # =========================================================
        # MAIN WINDOW
        # =========================================================

        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        main_layout = QVBoxLayout(central_widget)
        main_layout.setContentsMargins(30, 25, 30, 25)
        main_layout.setSpacing(20)

        # =========================================================
        # HEADER
        # =========================================================

        title = QLabel("RSA CRYPTOGRAPHY SIMULATOR")
        title.setAlignment(Qt.AlignCenter)

        title_font = QFont()
        title_font.setPointSize(24)
        title_font.setBold(True)
        title.setFont(title_font)

        subtitle = QLabel("Educational RSA Implementation")
        subtitle.setAlignment(Qt.AlignCenter)

        subtitle_font = QFont()
        subtitle_font.setPointSize(11)
        subtitle.setFont(subtitle_font)

        main_layout.addWidget(title)
        main_layout.addWidget(subtitle)

        # =========================================================
        # KEY GENERATION CARD
        # =========================================================

        key_card = QFrame()
        key_card.setObjectName("card")

        key_layout = QVBoxLayout(key_card)
        key_layout.setContentsMargins(25, 20, 25, 20)
        key_layout.setSpacing(15)

        key_title = QLabel("🔐  KEY GENERATION")

        key_title_font = QFont()
        key_title_font.setPointSize(16)
        key_title_font.setBold(True)
        key_title.setFont(key_title_font)

        key_layout.addWidget(key_title)

        # Grid for parameters

        grid = QGridLayout()
        grid.setHorizontalSpacing(15)
        grid.setVerticalSpacing(12)

        self.p_input = QLineEdit()
        self.q_input = QLineEdit()
        self.n_input = QLineEdit()
        self.phi_input = QLineEdit()
        self.e_input = QLineEdit()
        self.d_input = QLineEdit()

        # Make outputs read-only

        self.p_input.setReadOnly(True)
        self.q_input.setReadOnly(True)
        self.n_input.setReadOnly(True)
        self.phi_input.setReadOnly(True)
        self.e_input.setReadOnly(True)
        self.d_input.setReadOnly(True)

        grid.addWidget(QLabel("Prime p"), 0, 0)
        grid.addWidget(self.p_input, 0, 1)

        grid.addWidget(QLabel("Prime q"), 1, 0)
        grid.addWidget(self.q_input, 1, 1)

        grid.addWidget(QLabel("n = p × q"), 2, 0)
        grid.addWidget(self.n_input, 2, 1)

        grid.addWidget(QLabel("φ(n)"), 3, 0)
        grid.addWidget(self.phi_input, 3, 1)

        grid.addWidget(QLabel("Public Key (e, n)"), 4, 0)
        grid.addWidget(self.e_input, 4, 1)

        grid.addWidget(QLabel("Private Key (d, n)"), 5, 0)
        grid.addWidget(self.d_input, 5, 1)

        key_layout.addLayout(grid)

        self.generate_button = QPushButton("GENERATE RSA KEYS")
        self.generate_button.setMinimumHeight(45)

        self.generate_button.clicked.connect(self.generate_keys)

        key_layout.addWidget(self.generate_button)

        main_layout.addWidget(key_card)

        # =========================================================
        # MESSAGE CARD
        # =========================================================

        message_card = QFrame()
        message_card.setObjectName("card")

        message_layout = QVBoxLayout(message_card)
        message_layout.setContentsMargins(25, 20, 25, 20)
        message_layout.setSpacing(15)

        message_title = QLabel("✉  MESSAGE")

        message_title_font = QFont()
        message_title_font.setPointSize(16)
        message_title_font.setBold(True)
        message_title.setFont(message_title_font)

        message_layout.addWidget(message_title)

        message_label = QLabel("Enter your message:")

        message_layout.addWidget(message_label)

        self.message_input = QTextEdit()
        self.message_input.setPlaceholderText(
            "Write the message you want to encrypt..."
        )
        self.message_input.setMinimumHeight(100)

        message_layout.addWidget(self.message_input)

        # Buttons

        buttons_layout = QHBoxLayout()
        buttons_layout.setSpacing(15)

        self.encrypt_button = QPushButton("🔒  ENCRYPT")
        self.decrypt_button = QPushButton("🔓  DECRYPT")

        self.encrypt_button.setMinimumHeight(45)
        self.decrypt_button.setMinimumHeight(45)

        self.encrypt_button.clicked.connect(self.encrypt_message)
        self.decrypt_button.clicked.connect(self.decrypt_message)

        buttons_layout.addWidget(self.encrypt_button)
        buttons_layout.addWidget(self.decrypt_button)

        message_layout.addLayout(buttons_layout)

        main_layout.addWidget(message_card)

        # =========================================================
        # ENCRYPTED MESSAGE
        # =========================================================

        encrypted_card = QFrame()
        encrypted_card.setObjectName("card")

        encrypted_layout = QVBoxLayout(encrypted_card)
        encrypted_layout.setContentsMargins(25, 20, 25, 20)

        encrypted_title = QLabel("🔒  ENCRYPTED MESSAGE")

        encrypted_title_font = QFont()
        encrypted_title_font.setPointSize(16)
        encrypted_title_font.setBold(True)
        encrypted_title.setFont(encrypted_title_font)

        encrypted_layout.addWidget(encrypted_title)

        self.encrypted_output = QTextEdit()
        self.encrypted_output.setReadOnly(True)
        self.encrypted_output.setPlaceholderText(
            "Encrypted message will appear here..."
        )
        self.encrypted_output.setMaximumHeight(100)

        encrypted_layout.addWidget(self.encrypted_output)

        main_layout.addWidget(encrypted_card)

        # =========================================================
        # DECRYPTED MESSAGE
        # =========================================================

        decrypted_card = QFrame()
        decrypted_card.setObjectName("card")

        decrypted_layout = QVBoxLayout(decrypted_card)
        decrypted_layout.setContentsMargins(25, 20, 25, 20)

        decrypted_title = QLabel("🔓  DECRYPTED MESSAGE")

        decrypted_title_font = QFont()
        decrypted_title_font.setPointSize(16)
        decrypted_title_font.setBold(True)
        decrypted_title.setFont(decrypted_title_font)

        decrypted_layout.addWidget(decrypted_title)

        self.decrypted_output = QTextEdit()
        self.decrypted_output.setReadOnly(True)
        self.decrypted_output.setPlaceholderText(
            "Decrypted message will appear here..."
        )
        self.decrypted_output.setMaximumHeight(100)

        decrypted_layout.addWidget(self.decrypted_output)

        main_layout.addWidget(decrypted_card)

        # =========================================================
        # STATUS BAR
        # =========================================================

        self.status_label = QLabel(
            "Status: Ready. Generate RSA keys to begin."
        )

        self.status_label.setAlignment(Qt.AlignCenter)

        main_layout.addWidget(self.status_label)

    # =============================================================
    # BUTTON FUNCTIONS
    # =============================================================

    def generate_keys(self):

        self.p_input.setText("Generating...")
        self.q_input.setText("Generating...")

        self.status_label.setText(
            "Status: Generating RSA keys with C++ backend..."
        )
        QApplication.processEvents()

        data = self.run_cpp_backend(["--generate-keys"])
        if not data:
            self.status_label.setText("Status: Key generation failed.")
            return

        self.p_val = data["p"]
        self.q_val = data["q"]
        self.e_val = data["e"]
        self.d_val = data["d"]
        self.n_val = data["n"]

        self.p_input.setText(self.p_val)
        self.q_input.setText(self.q_val)
        self.n_input.setText(self.n_val)
        self.phi_input.setText(data["phi"])
        self.e_input.setText(f"({self.e_val}, {self.n_val})")
        self.d_input.setText(f"({self.d_val}, {self.n_val})")

        self.status_label.setText(
            "Status: ✓ RSA keys generated successfully."
        )

    def encrypt_message(self):

        message = self.message_input.toPlainText().strip()

        if not message:
            QMessageBox.warning(
                self,
                "No Message",
                "Please enter a message to encrypt."
            )
            return

        if not self.p_val or not self.q_val or not self.e_val:
            QMessageBox.warning(
                self,
                "No Keys",
                "Please generate RSA keys first."
            )
            return

        data = self.run_cpp_backend([
            "--encrypt",
            message,
            self.p_val,
            self.q_val,
            self.e_val
        ])

        if data:
            self.encrypted_output.setText(data["encrypted"])
            self.status_label.setText(
                "Status: ✓ Message encrypted successfully."
            )

    def decrypt_message(self):

        encrypted_message = (
            self.encrypted_output.toPlainText().strip()
        )

        if not encrypted_message:
            QMessageBox.warning(
                self,
                "No Encrypted Message",
                "Please encrypt a message first."
            )
            return

        if not self.p_val or not self.q_val or not self.d_val:
            QMessageBox.warning(
                self,
                "No Keys",
                "Missing keys required for decryption."
            )
            return

        data = self.run_cpp_backend([
            "--decrypt",
            encrypted_message,
            self.p_val,
            self.q_val,
            self.d_val
        ])

        if data:
            self.decrypted_output.setText(data["decrypted"])
            self.status_label.setText(
                "Status: ✓ Message successfully decrypted."
            )


# ================================================================
# APPLICATION
# ================================================================

if __name__ == "__main__":

    app = QApplication(sys.argv)

    app.setStyleSheet("""
        QMainWindow {
            background-color: #101318;
        }

        QWidget {
            color: #E8EAED;
            font-family: "Segoe UI";
            font-size: 14px;
        }

        QFrame#card {
            background-color: #181C23;
            border: 1px solid #2A303A;
            border-radius: 12px;
        }

        QLineEdit {
            background-color: #0F1217;
            border: 1px solid #303640;
            border-radius: 7px;
            padding: 10px;
            color: #E8EAED;
        }

        QLineEdit:focus {
            border: 1px solid #5B8DEF;
        }

        QTextEdit {
            background-color: #0F1217;
            border: 1px solid #303640;
            border-radius: 7px;
            padding: 10px;
            color: #E8EAED;
        }

        QTextEdit:focus {
            border: 1px solid #5B8DEF;
        }

        QPushButton {
            background-color: #2D6CDF;
            border: none;
            border-radius: 7px;
            padding: 10px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #3B7BEA;
        }

        QPushButton:pressed {
            background-color: #2458B8;
        }

        QLabel {
            background-color: transparent;
        }
    """)

    window = RSAInterface()
    window.show()

    sys.exit(app.exec())