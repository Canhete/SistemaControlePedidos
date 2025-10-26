# Estrutura de diretórios inicial
ATENÇÃO: ESSE ARQUIVO README.md SERÁ EDITADO FUTURAMENTE PARA FAZER A DOCUMENTAÇÃO DO PROGRAMA
MAS ENQUANTO ISSO NÃO ACONTECE USAREMOS ELE COMO `BLUEPRINT`

SistemaControlePedidos/
│
├── include/
│ ├── cliente.h
│ ├── produto.h
│ ├── pedido.h
│ ├── interface.h
│ └── persistencia.h
│
├── src/
│ ├── cliente.c
│ ├── produto.c
│ ├── pedido.c
│ ├── interface.c
│ ├── persistencia.c
│ └── main.c
│
├── data/
│ ├── Clientes.csv
│ ├── Produtos.csv
│ └── Pedidos.csv
│
├── build/
│
└── Makefile

Função Responsabilidades principais GitHub
Líder técnico: Raphael -> Gerencia o repositório, define estrutura e revisa código. Controla branch principal `main`
Desenvolvedor 1: Guilherme -> Implementa módulo Cliente. Trabalha em branch `feature-cliente`
Desenvolvedor 2: Lays -> Implementa módulo Produto. Trabalha em branch `feature-produto`
Desenvolvedor 3: Raphael -> Implementa módulo Pedido e integração. Trabalha em branch `feature-pedido`
