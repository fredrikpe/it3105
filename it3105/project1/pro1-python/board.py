from PyQt5 import QtCore

from PyQt5 import QtGui

from PyQt5 import QtWidgets


class Board(QtWidgets.QWidget):
    def __init__(self, parent, board_size=4):
        super().__init__()
        self.parent = parent
        self.queens = []
        self.board_size = board_size

    def paintEvent(self, QPaintEvent):
        rect = QPaintEvent.rect()
        painter = QtGui.QPainter(self)
        painter.setRenderHint(QtGui.QPainter.Antialiasing)

        square_width = rect.width() / self.board_size
        square_height = rect.height() / self.board_size

        painter.setFont(QtGui.QFont("times", square_height / 2))

        for i in range(self.board_size):
            for j in range(self.board_size):
                if (i % 2 and not(j % 2)) or (i + j) % 2:
                    painter.fillRect(rect.left() + i * square_width, rect.top() + j * square_height,
                                     square_width, square_height, QtCore.Qt.lightGray)
                else:
                    painter.fillRect(rect.left() + i * square_width, rect.top() + j * square_height,
                                     square_width, square_height, QtCore.Qt.white)

                if len(self.queens) > i and self.queens[i] == j:
                    if len(self.queens) == self.board_size and self.is_valid(self.queens):
                        painter.setPen(QtCore.Qt.blue)
                    else:
                        painter.setPen(QtCore.Qt.red)

                    painter.drawText(rect.left() + i * square_width + square_width/4,
                                     rect.top() + j*square_height + 2*square_height/3, "Q")

    def is_valid(self, queens):
        if len(queens) > self.board_size:
            raise Exception("Queens list longer than board size.")

        # Horizontal conflict (list contains duplicates).
        if len(queens) != len(set(queens)):
            return False

        # Diagonal conflict (x and y distance is equal).
        diagonal_conflicts = 0
        for i in range(len(queens)):
            for j in range(len(queens)):
                if i == j:
                    continue

                y_dist = abs(queens[i] - queens[j])
                x_dist = abs(i - j)

                if y_dist == x_dist:
                    return False

        return True


