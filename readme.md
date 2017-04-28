# 2584 AI
This is an variant of 2048 game.
In this project, the AI use TD(lambda) Learning and Alpha-Beta search to 
implement two AI. One is a solver to maximize the tile, and the other is an evil preventing high score tiles from generating.

## Installation

First lauch the platform (see platform/readme.md)
```
git clone https://github.com/tonylearn09/2584AI.git
cd 2584AI/
make
```

## Usage
* To train the AI:
`
./train_game [Numbers of iteration] -t
`


* To run the AI:
`
./ai_client [account] [password]
`
