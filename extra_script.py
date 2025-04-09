Import("env")
# import datetime

# timestamp = datetime.datetime.now().timestamp()
# date = datetime.datetime.fromtimestamp(timestamp)

# date_str = date.strftime("%Y%m%d_%H%M%S")

version = env.GetProjectOption("custom_prog_version")
softversion = env.GetProjectOption("custom_soft_version")
# firmware_name = f"firmware_{version}_{date_str}"
firmware_name = f"firmware_{version}_{softversion}"
#固件名称
env.Replace(PROGNAME=firmware_name)
# 将version写入文件中
version_json = '{"version": "' + softversion + '"}'
with open('version.json', 'w') as f:
    f.write(version_json)
#打开library.properties文件,查找version，替换version为当前版本号

with open('library.properties', 'r') as f:
    lines = f.readlines()
with open('library.properties', 'w') as f:
    for line in lines:
        if line.startswith('version='):
            #去除softversion前面的"V"
            softversion = softversion.replace("V", "")
            #替换版本号
            f.write(f'version={softversion}\n')
        else:
            f.write(line)
    


