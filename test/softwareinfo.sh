#!/bin/bash
# 软件信息采集

:<<！
先获取路径
判断用到的命令是否存在
init文件中的格式内容如下：
[004]
dpkg_name=nginx
rpm_name=nginx
exe=/usr/sbin/nginx
software_name=Nginx
software_type=0
version_cmd=/usr/sbin/nginx -v 2>&1 | awk '{print $3}' | awk -F '/' '{print $2}'
company=Nginx, Inc
！

SCRIPTPATH=$(cd $(dirname $0);pwd -P)      #脚本路径 

#设置运行环境为英文                                                        没有设置为英文。。。。。。。。。。。。
export LANG=""
export LANGUAGE=""

# rpm 命令是否存在
rpm -qa >/dev/null 2>&1    #将out err 合并后 重定向到/dev/null
existRpm=$?                #获得之前的执行结果  注意0是真  1是假

#dpkg 命令是否存在
type dpkg-query >/dev/null 2>&1    #软件包查询命令                             type的作用是什么？？？？？
existDpkg=$?

##  获得 ini 文件所有的 sections 名称（注意：不同 sections 之间要有空白换行）
##  list_ini_sections   "filename.ini"
list_ini_sections() {            
    local inifile="$1"            
    if [ ! -f "$inifile" ]; then  #是否是普通文件   写成一行加; 不然then在下一行
        echo ""                   #不是  不能不写命令  所以写一个echo ""
    else
        sed -n '/\[*\]/p' "${inifile}" | grep -v '^#'| tr -d []              #正则表达式的匹配  记得复习一下
    fi
}


## 在 ini 文件中，根据 section 和 key 获得对应 value 值（注意：value 不能有等号）
## get_ini_value "filename.ini" "section" "key"
get_ini_value() {
    local inifile="$1"
    local section="$2"   
    local key="$3"
    if [ ! -f "$inifile" ]; then
        echo ""
    else #是普通文件
        sed -n '/\['"$section"'\]/,/^$/p' "$inifile" | grep -Ev '\[|\]|^$' | awk -F '=' '$1 == "'"$key"'" {print $2}'  #？？
    fi
}

clear_while_variables() {  #clean作用
    rpm_name=""
    exe=""
    software_name=""
    software_type=""
    version_cmd=""
    version=""
    company=""
    install_time=0
    install_path=""
    host_type=1
    record_id=""
}
iinifile="$SCRIPTPATH/../config/software.ini"  #获得要扫描的文件的路径

while read -r section; do  #读每一行  就是一个标号

    clear_while_variables  #调用函数

    # 先尝试通过 rpm 命令获取 version 和 install_time
    if [[ $existRpm == 0 ]]; then
        rpm_name=$(get_ini_value "$iinifile" "$section" rpm_name)
        if [[ $rpm_name != "" ]]; then
            read -r version install_time < <(rpm -qa --queryformat "%{NAME} %{VERSION} %{INSTALLTIME}\n" | grep "$rpm_name" | awk '$1 == "'"$rpm_name"'" {print $2,$3}')
            #按格式查找  grep出想要的软件名的那一行  用awk  
        fi
    fi

    if [[ $existDpkg == 0 ]]; then   #软件包查询命令
        dpkg_name=$(get_ini_value "$iinifile" "$section" dpkg_name)
        if [[ $dpkg_name != "" ]]; then
            read -r version < <(dpkg-query -W -f='${Version}\n' "$dpkg_name" 2>/dev/null)  #dpkg的参数
        fi
    fi

    # 白名单获取软件、版本    直接利用ini里的cmd来获取version
    exe=$(get_ini_value "$iinifile" "$section" exe)
    if ls $exe > /dev/null 2>&1; then
        install_path=$(ls $exe | tail -1) 
    fi
    if [[ $version == "" && $install_path != "" ]]; then
        version_cmd=$(get_ini_value "$iinifile" "$section" version_cmd)   
        version=$(eval "$version_cmd")
    fi

    # 无法获得 version 则说明没有此软件
    if [[ $version == "" ]]; then
        continue
    fi

    # 无法获得 install_time 则默认置 0
    if [[ $install_time == "" ]]; then
        install_time=0
    fi

    software_name=$(get_ini_value "$iinifile" "$section" software_name)
    software_type=$(get_ini_value "$iinifile" "$section" software_type)
    company=$(get_ini_value "$iinifile" "$section" company)
    host_type=1
    record_id=$(echo -n "$software_name""$version" | md5sum | cut -d ' ' -f1)
    data_md5=$(echo -n "$software_type""$company""$install_time""$install_path" | md5sum | cut -d ' ' -f1)

    echo "{\"software_name\":\"$software_name\",\"software_type\":$software_type,\
\"version\":\"$version\",\"company\":\"$company\",\"install_time\":$install_time,\
\"install_path\":\"$install_path\",\"host_type\":$host_type,\"record_id\":\"$record_id\", \"data_md5\":\"$data_md5\"}"

done < <(list_ini_sections "$iinifile")  # 将括号里的内容驶入到while循环中  为何是< <  是这个函数输出到1  1再重定向

#看一下MD5sum的参数  参数是要加密的东西  之后得到MD5码吗

# EDR 特殊处理 
clear_while_variables

software_name="EDR终端防护中心"
software_type=3
company="Sangfor Technologies Inc."
host_type=1
install_path=$(dirname "$SCRIPTPATH") # /sangfor/edr/agent
install_time=$(stat -c %Z "$install_path"/bin/edr_agent)
main_version=$(grep -Eo "[0-9]*\.[0-9]*\.[0-9]*" "$install_path"/config/agent_cfg.json | head -1) # 3.2.17
plus_version=$(grep -Eo "EN|EN_B|R1|R1_B|_B" "$install_path"/config/agent_cfg.json | head -1) # 3.2.17
version="${main_version}${plus_version}"
record_id=$(echo -n "$software_name""$version" | md5sum | cut -d ' ' -f1)
data_md5=$(echo -n "$software_type""$company""$install_time""$install_path" | md5sum | cut -d ' ' -f1)

echo "{\"software_name\":\"$software_name\",\"software_type\":$software_type,\
\"version\":\"$version\",\"company\":\"$company\",\"install_time\":$install_time,\
\"install_path\":\"$install_path\",\"host_type\":$host_type,\"record_id\":\"$record_id\", \"data_md5\":\"$data_md5\"}"
