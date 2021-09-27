import chess
import chess.engine
import serial
import time

class chessGame():
    def __init__(self, path='/home/pi/AICode/stockfish', difficulty=20):
        self.engine = chess.engine.SimpleEngine.popen_uci(path)
        self.board = chess.Board()
        self.cpu_turn = False

        self.engine.configure({"Skill Level": difficulty}) # can be 0-20

    def send_move(self, move):
        if type(move) is not chess.Move:
            if move[0] and move [2] not in ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']:
                return False
            if move[1] and move[3] not in ['1', '2', '3', '4', '5', '6', '7', '8']:
                return False
            move = chess.Move.from_uci(move)
        if move not in self.board.legal_moves:
            return False
        self.board.push(move)
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
            print("Not Valid CPU Move!")
            cpu_move = game.get_best_move()
        uart.send_data(str(cpu_move) + '-')
    else:
        player_move = uart.receive_data()
        while not game.send_move(player_move):
            print("Not Valid Move!")
            time.sleep(0.5)
            uart.send_data('-----')
            player_move = uart.receive_data()

    game.change_turn()

game.engine.quit()
