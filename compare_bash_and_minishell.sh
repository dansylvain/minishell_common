#!/bin/bash

# Définir le chemin de votre exécutable Minishell
MINISHELL_EXECUTABLE="./minishell"

# Définir les chemins des fichiers de sortie
BASH_OUTPUT_FILE="bash_output.txt"
MINISHELL_OUTPUT_FILE="minishell_output.txt"

# Vider les fichiers de sortie avant chaque exécution
> "$BASH_OUTPUT_FILE"
> "$MINISHELL_OUTPUT_FILE"

# Liste des commandes à tester
commands=(
    "/bin/ls"
	"echo $?"
	""
	"echo $?"
	"/bin/ls -al"
	"echo $?"
	"grep -i 'return' src/main.c"
	"echo $?"
	"/usr/bin/cat testfile"
	"echo $?"
	"/bin/grep -i motif /testfile"
	"echo $?"
	"/bin/chmod 755 /testfile"
	"echo $?"
	"/bin/cp /testfile /includes/mon_fichier.txt"
	"echo $?"
	"/bin/mv /testfile /includes/mon_fichier.txt"
	"echo $?"
	"echo bonjour"
	"echo $?"
	"echo     	"
	"echo $?"
	"echo"
	"echo $?"
	"echo -n au revoir"
	"echo $?"
	"ls -a | rev"
	"echo $?"
	"exit 42"
	"echo $?"
	"exit 42hgzef"
	"echo $?"
	"exit 42 jhgzegf"
	"echo $?"
	"exit jhzef"
	"echo $?"
	"ls -a"
	"echo $?"
	"<noexist cat"
	"echo $?"
	"bin/ls fichiernulachier"
	"echo $?"
	"echo $? + $?"
	"echo $?"
	"ls "includes""
	"echo $?"
	"echo \"cat lol.c | cat > lol.c\""
	"echo $?"
	"ls inclu\"des\""
	"echo $?"
	"l's' '-'"l""
	"echo $?"
	"ls """
	"echo '$USER'"
	"echo $?"
	"cat includes/minishell.h"
	"echo $?"
	# "cat <includes/minishell.h"
	"echo $?"
	"echo papa >> file1"
	"echo $?"
	"echo maman >> file1"
	"echo $?"
	"cat file1"
	"echo $?"
	"cat file1 | grep papa | more"
	"echo $?"
	"ls fichiernul | grep bla | more"
	"echo $?"
	"export VAR=tamaman"
	"echo $?"
	"echo $VAR"
	"echo $?"
	"echo $VAR"
	"unset VAR"
	"echo $?"
	"echo $VAR"
	"echo $?"
	"unset PATH"
	"echo $?"
	"ls"
	"echo $?"
	"cd .."
	"pwd"
	"echo $?"
	"cd"
	"echo $?"
	"pwd"
	"echo $?"
	"pwd kzbgkbzekgb"	
	"echo $?"
	"pwd kzbg  kbzekgb"	
	"echo $?"
	"pwd 42 bzekgb"	
	"echo $?"
	"cat includes/minishell.h"
	"echo $?"
	"cat ../../42/minishell_common/includes/minishell.h"
	"echo $?"
	"<testfile cat | rev > outfile"
	"echo $?"
	"cat <testfile"
	"echo $?"
	"<outfile cat"
	"echo $?" 
	"<testfile cat | echo niania | rev > outfile"
	"echo $?"
	"<outfile cat"
	"echo $?"
	"<testfile cat | echeo niania | rev > outfile"
	"echo $?"
	"<outfile cat"
	"echo $?" 
	"echo aaa && echo bbb || echo ccc"
	"echo $?"
	"echo aaa || echo bbb || echo ccc"
	"echo $?"
	"echo $?"
	"false && echo bbb || echo ccc"
	"echo $?"
	"false || echo bbb || echo ccc"
	"echo $?"
	"false && echo bbb && echo ccc"
	"echo $?"
	"echo aaa && false && echo ccc"
	"echo $?"
	"<<end <<end"
	"<<end cat"
	"<<end" 
	#"cat | cat | ls"
	"echo $?"
	
	"echo $SHELL"
	"echo $?"
	"echo $LOGNAME$SHELL$"
	"echo $?"
	"echo $LOGNAME$SHELL$ju"
	"echo $?"
	"echo $LOGNAME   $SHELL $"
	"echo $?"
	"echo '$LOGNAME$SHELL$'"
	"echo $?"
	"echo $LOGNAME $SHELL $ $tr"
	"echo $?"
	"echo "$LOGNAME$SHELL$""
	"echo $?"
	"echo \"$LOGNAME$  SHELL $ $thu\""
	"echo $?"
	"echo \"trrt$LOGNAME$  $SHELLtruc SHELL $ $thu\""
	"echo $?"
	"echo \"trrt$LOGNAME$hyu  $SHELLtruc SHELL $ $thu\""
	"echo $?"
	"echo \"trrt$LOGNAME$hyufr$SHELLtruc SHELL $ $thu\""
	"echo $?"
	"echo trrt$LOGNAME$  $SHELLtruc SHELL $ $thu"
	"echo $?"
	"echo \"trrt$LOGNAME'Mach' $'LOGNAME' $SHELLtruc SHELL $ $thu\""
	"echo $?"
	"echo \"trrt$LOGNA'Mach' $LOGNAME'Mach'$LOG $'LOGNA'M''E' $SHELLtruc SHELL $ $thu\""
	"echo $?"
	"echo \"trrt$LOGNA'Mach'p lou$LOGNAME'Mach'$LOG $'LOGNA'M''E'42 $SHELLtruc SHELL $ $thu\""
	"echo $?"
	"echo \"$USER\""
	"echo $?"
	"echo $USER"
	"echo $?"
)

# Fonction pour comparer les sorties
compare_outputs() {
    command="$1"
    command_output=$(eval "$command" 2>&1)
    minishell_output=$($MINISHELL_EXECUTABLE <<< "$command" 2>&1 | tail -n +2 | sed '$d')

 	# Filtrer les sorties pour ignorer les différences non significatives
    filtered_command_output=$(echo "$command_output" | sed 's/ //g')
    filtered_minishell_output=$(echo "$minishell_output" | sed 's/ //g')
    

    if [ "$filtered_command_output" != "$filtered_minishell_output" ]; then
        echo "Différence détectée pour la commande: $command"
        echo "Sortie Minishell: $minishell_output"
        echo "Sortie Bash: $command_output"
        echo "---------------------------------------"
    fi
	# Ajouter les sorties dans les fichiers correspondants
    echo "Commande: $command" >> "$BASH_OUTPUT_FILE"
    echo "$command_output" >> "$BASH_OUTPUT_FILE"
    echo "" >> "$BASH_OUTPUT_FILE"

    echo "Commande: $command" >> "$MINISHELL_OUTPUT_FILE"
    echo "$minishell_output" >> "$MINISHELL_OUTPUT_FILE"
    echo "" >> "$MINISHELL_OUTPUT_FILE"
}

# Exécutez les commandes et comparez les sorties
for cmd in "${commands[@]}"; do
    compare_outputs "$cmd"
done

echo "Comparaison terminée."
