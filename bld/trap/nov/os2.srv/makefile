#pmake: serv remote os_os2 cpu_i86 nov

tree_depth = 4

host_cpu = i86
host_os  = os2

target_cpu = x86

srv = nov
which = SERVER
remote_link = 1

extra_objs = novos2.obj
inc_dirs = -I"$(nwos2dir)"
extra_libs = $(nwos2dir)/ipxcalls.lib $(nwos2dir)/nwcallsp.lib $(nwos2dir)/spxcalls.lib

!include $(trap_dir)/master.mif
