#Terminal MQTT CLIENT

##安装Tengine 编译参数
```shell
./configure --prefix=/usr/local/terminal
```

## 配置wangyonglin开机启动，切换到/lib/systemd/system目录,创建 terminal.service文件：
```shell
[Unit]
Description=terminal
After=network.target
[Service]
Type=forking
ExecStart=/usr/local/terminal/bin/terminal
ExecReload=/usr/local/terminal/bin/terminal reload
ExecStop=/usr/local/terminal/bin/terminal quit
PrivateTmp=true
[Install]
WantedBy=multi-user.target
```
