#!/bin/bash

DISK_NAME="disk0.dsk"
EXEC_NAME="./myfs" 
FS_ID="1" 

echo "=========================================="
echo "      INICIANDO AUTOMACAO DE TESTES       "
echo "=========================================="

echo "[1/3] Limpando arquivos antigos..."
rm -f $DISK_NAME
make clean > /dev/null

echo "[2/3] Compilando o projeto..."
make all 
if [ $? -ne 0 ]; then
    echo "Erro: Falha na compilação!"
    exit 1
fi

echo "[3/3] Rodando o simulador com inputs automáticos..."
echo "------------------------------------------"

$EXEC_NAME <<EOF
d           
b           
$DISK_NAME  
100         
c           
$DISK_NAME  
<           
f           
f           
0           
$FS_ID      
1           
m           
0           
$FS_ID      
<           
r           
o           
/           
a           
1           
meuarquivo.txt
123         
l           
1           
<           
i           
o           
/meuarquivo.txt
w           
2           
40          
r           
2           
40          
c           
2           
<           
r           
r           
1           
meuarquivo.txt
l           
1           
c           
1           
<           
f           
u           
<           
d           
d           
0           
<           
q           
EOF

echo "------------------------------------------"
echo "Teste finalizado."
