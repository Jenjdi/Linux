# 代码修复与验证说明

## 本次修改

修复日志输出模式和月份、重连状态访问器、线程回调类型、HTTP 接收长度/端口字节序/路由/正文/响应长度及缺失的头文件依赖。Reactor 使用单个 epoll 循环管理连接，移除未创建的 eventfd 和不安全的并发连接表操作。Reactor_v2 仍是教学骨架，仅修复类型声明和包含关系，不代表完整服务。

修改处使用中文注释解释原因；新增及重写的变量使用能够表达业务含义的名称，保留未涉及的公开接口和题目约定。部分教学代码仍不是生产级组件。

## 运行检查

在本仓库根目录执行，要求 GCC/G++（C11、C++17）、Python 3。Protobuf、VideoWebSite 和部分 Linux 示例还需要 Protobuf 3.21、JsonCpp、MariaDB 客户端开发库。云环境可先执行 `source /workspace/.setup/env.sh`，再设置：

```bash
export CPPFLAGS=-I/workspace/.setup/sysroot/usr/include
export LDFLAGS=-L/workspace/.setup/sysroot/usr/lib/x86_64-linux-gnu
python3 tests/check_syntax.py
```

通用系统安装路径可以不设置 CPPFLAGS/LDFLAGS。语法检查覆盖每个源文件及独立头文件，不等同于算法正确性验证。

回归测试：

```bash
set -e
for test in socket http; do
  g++ -std=c++17 -pthread -fsanitize=address,undefined -g tests/$test.cpp -o /tmp/linux-$test
  /tmp/linux-$test || exit 1
done
```

