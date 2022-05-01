#DS143_Trabalho

Para compilar o arquivo main.c, deve-se acessar, via terminal, a pasta de destino do arquivo e usar o comando 
"gcc -o indexer main.c", o qual cria um executável de nome "indexer".

Para executar o arquivo gerado, utilize o comando:
"./indexer [--freq N ARQUIVO/ --freq-word TERMO ARQUIVO/ --search TERMO ARQUIVO [ARQUIVO_1, ARQUIVO_2... ARQUIVO_N]]".

"N" é um número inteiro, referente aos termos mais relevantes dentro de um dado arquivo;
"ARQUIVO" é uma String, referente ao caminho absoluto de um arquivo (por exemplo "C:/Users/Usuario/Documents/texto.txt");
"TERMO" é uma palavra, a qual será contada para responder às requisições da segunda e terceira opções;
"[ARQUIVO_1, ARQUIVO_2... ARQUIVO_N]" é uma lista de arquivos, acessados pelo seu caminho absoluto.

Nota: Apenas a opção 3, --search, é capaz de reconhecer mais de um arquivo. As demais lerão apenas o primeiro arquivo
passado como parâmetro, ou seja, ao se digitar 
"indexer --freq 100 'C:/Users/Usuario/Documents/texto1.txt' 'C:/Users/Usuario/Documents/texto2.txt'", apenas o 
texto1.txt será lido.
