import chess
import chess.engine
import serial

port = serial.Serial("/dev/ttyS0", baudrate=9600)

engine = chess.engine.SimpleEngine.popen_uci("stockfish")

board = chess.Board()
print(board)

engine.configure({"Skill Level": 20}) # can be 0-20

cpu_turn = True

while not board.is_game_over() and not board.is_stalemate():
    if cpu_turn:
        result = engine.play(board, chess.engine.Limit(time=2))
        board.push(result.move)
        print(result.move)
        print(board)
        move = str.encode(str(result.move))
        port.write(b"\rAI Move: ")
        port.write(move)
        port.write(b'\n')
        
    else:
        try:
            port.write(b"\rMove: ")
            move_in = port.read(4)
            port.write(move_in)
            port.write(b'\n')
            print(move_in.decode("utf-8"))

            move = chess.Move.from_uci(move_in.decode("utf-8"))
            board.is_valid()
            board.push(move)
            if not board.is_valid():
                board.pop()
                raise Exception
            print(board)
        except:
            print("Must be a valid move!")
            cpu_turn = not cpu_turn

    #cpu_turn = not cpu_turn
        

print("Game Over!")

engine.quit()