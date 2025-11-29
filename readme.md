# Projeto
Implementação Pseudo-SO na linguagem C para disciplina Sistema Operacionais da Universidade de Brasília

### Alunos
Arthur Motta Furtado
Gabriel Cruz Vaz Santos
Nicolas Bennato Paulin

### Como rodar projeto
Dentro do diretório do projeto rodar:
`make`

Para criar o compilável. Após a criação rodar:
`./program nome_arquivo_1 nome_arquivo_2`

Substituindo nome_arquivo pelos respectivos arquivos de teste do Pseudo SO.

### Como o sistema funciona
- Faz a leitura do primeiro arquivo e cria as estruturas do processos;
- Chama scheduler para criar as filas;
- Roda e imprimir todos os processos nas filas;
- Lê o segundo arquivo executando suas respectivas operações;