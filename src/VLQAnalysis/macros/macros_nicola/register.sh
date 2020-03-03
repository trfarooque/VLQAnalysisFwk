#TOP production
#user.mcasolin:user.mcasolin*.HtX4Tops_00-00-12_out.root

#Latest MB production
#user.lvalery:user.lvalery*.*_2.4.24-1-0_output_tree.root
#user.farooque:user.farooque*.*_2.4.24-1-0_output_tree.root
#user.juste:user.juste*.*_2.4.24-1-0_output_tree.root      
#user.dyamaguc:user.dyamaguc*.*_2.4.24-1-0_output_tree.root

list_patterns="user.farooque:user.farooque*.*_2.4.24-1-0_output_tree.root
user.dyamaguc:user.dyamaguc*.*_2.4.24-1-0_output_tree.root
user.juste:user.juste*.*_2.4.24-1-0_output_tree.root
user.gerbaudo:user.gerbaudo.4106*.FCNC.AOD.*_2.4.22-4-0_output_tree.root"

#list_patterns="user.mcasolin:user.mcasolin*.HtX4Tops_00-00-12_out.root"

for pattern in ${list_patterns}
do

    echo "Copying files under the pattern ${pattern}"
    for  X in  $(rucio ls "${pattern}" --filter type=container --short)       
    do
	rucio add-rule $X 1 'CERN-PROD_SCRATCHDISK' 2>&1 | tee --append rules_cern_log.txt 
    done
    
    #rucio ls "${pattern}"  | grep root  | sed -r "s/\| CONTAINER    \|//" | sed -r "s/\| user.dyamaguc://" | sed -r "s/\| user.juste://" | sed -r "s/\| user.farooque://" | sed -r "s/\| user.lvalery://" | sed -r "s/\| user.gerbaudo://"

done