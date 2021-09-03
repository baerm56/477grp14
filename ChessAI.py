import chess
import chess.engine
import serial

class chessGame():
    def __init__(self, path='stockfish', difficulty=20):
        self.engine = chess.engine.SimpleEngine.popen_uci(path)
        self.board = chess.Board()
        self.cpu_turn = False

        self.engine.configure({"Skill Level": difficulty}) # can be 0-20

    def send_move(self, move):
        if type(move) is not chess.Move:
            move = chess.Move.from_uci(move)
        self.board.push(move)
        if not self.board.is_valid():
            self.board.pop()
            return False
        return True

    def get_best_move(self, time=2):
        return self.engine.play(self.board, chess.engine.Limit(time=time)).move

    def check_game_over(self):
        return self.board.is_game_over()
        
    def get_board(self):
        print(self.board)
        print('')

    def change_turn(self):
        self.cpu_turn = not self.cpu_turn

class serialConnection():
    def __init__(self, baudrate=9600):
        self.port = serial.Serial("/dev/ttyS0", baudrate=baudrate)

    def send_data(self, data):
        byte_data = str.encode(data)
        self.port.write(byte_data)

    def receive_data(self):
        byte_data = self.port.read(4)
        data = byte_data.decode("utf-8")
        return data


uart = serialConnection()
game = chessGame()

while not game.check_game_over():
    game.get_board()
    if game.cpu_turn:
        cpu_move = game.get_best_move()
        while not game.send_move(cpu_move):
            print("Not Valid Move!")
            cpu_move = game.get_best_move()
        uart.send_data('\r\nAI Move: ' + str(cpu_move))
    else:
        uart.send_data("\r\nMove: ")
        player_move = uart.receive_data()
        uart.send_data(player_move)
        while not game.send_move(player_move):
            print("Not Valid Move!")
            uart.send_data("\r\nMove: ")
            player_move = uart.receive_data()
            uart.send_data(player_move)

    game.change_turn()

game.engine.quit()