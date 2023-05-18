def getMove(fen):
    import chess
    import chess.polyglot
    moves = ""
    board = chess.Board(fen)
    with chess.polyglot.open_reader("D:/DL/komodo.bin") as reader:
        for entry in reader.find_all(board):
            moves += str(entry.move) + " " + str(entry.weight) + " "
    return moves[:-1]