# PenTesting
Shell code injection via reverse_tcp connection exploit. 
The reverse_tcp exploit bypasses firewalls as the host initiates the connection (forward connection) to the threat actor.

x64/Debug/shellcodeinjector.exe is activated in the cmd along with the PID of the process targeted for shell injection, while msfconsole is awaiting with an open connection.

Shell code was generated from metasploit msfvenom, which is outdated and easily recognized by basic antivirus systems.

Ignore the winapi message box folder
