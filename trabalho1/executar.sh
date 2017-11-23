echo "abrindo o programa..." 
echo "testanto imagem pequena... --> darth.jpg"
echo "usando threads:"
echo "darthtr.jpg = darth.jpg * 3 th" | ./main 
echo "usando processos:"
echo "darthpr.jpg = darth.jpg * 3 pr" | ./main
echo "testando um unico comando de execucao em linhas..."
echo "darthlin.jpg = darth.jpg * 3 col" | ./main
echo "testando um unico comando de execucao em colunas..."
echo "darthcol.jpg = darth.jpg * 3" | ./main


#------

echo "testanto imagem grande... --> Lich.jpg"
echo "usando threads:"
echo "lichtr.jpg = Lich.jpg * 3 th" | ./main 
echo "usando processos:"
echo "lichpr.jpg = Lich.jpg * 3 pr" | ./main
echo "testando um unico comando de execucao em linhas..."
echo "lichlin.jpg = Lich.jpg * 3 col" | ./main
echo "testando um unico comando de execucao em colunas..."
echo "lichcol.jpg = Lich.jpg * 3" | ./main

