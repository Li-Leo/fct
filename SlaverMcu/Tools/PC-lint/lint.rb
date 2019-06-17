#!/usr/bin/env ruby
require 'optparse'
require 'ostruct'
require 'fileutils'
require 'pathname'

$self_name = File.basename(__FILE__, ".rb")

#帮助信息
$help_str = %Q{This is used to lint project.
Usage: #{$self_name} [option] [file]	}

#打印参数出错并退出
def param_error(detail_info = "")
    print "Input parameter error"
	if(detail_info != "")
		print ": "
		puts detail_info
	else
		puts "."
	end
    exit -1
end

#获取相对目录
def relative_path(path, relative_dir = nil)
	if !relative_dir
		relative_dir = Dir.pwd()
	end
	new_path = Pathname(path).relative_path_from(Pathname(relative_dir)).to_s
	return new_path
end

#初始化所有option
$options = OpenStruct.new
# $options.outfile_name = nil
#$options.project_path = nil
$options.src_file_path = nil
begin
	#解析option,其中-h,--help,-v,--version是自带支持的,可以不用添加
	OptionParser.new do |opts|
		#设置程序的帮助信息
		opts.banner = $help_str
		#设置程序的版本信息
		opts.version = "1.0.0"
		
		# 第一项是Short option，第二项是Long option，第三项是对Option的描述
		# opts.on("-o", "--out=out_file",
		# "output to this file") do |file_name|
		# 	$options.outfile_name = file_name
		# end

		# 第一项是Short option，第二项是Long option，第三项是对Option的描述
		# opts.on("-p", "--project_path=project_path",
		# "the project path") do |path|
		# 	$options.project_path = path
		# end

		# 第一项是Short option，第二项是Long option，第三项是对Option的描述
		opts.on("-s", "--source=source_file_path",
		"the source file path") do |path|
			$options.src_file_path = path
		end

		opts.separator "Example:"
		opts.separator "#{$self_name} env-si.lnt "
		opts.separator "#{$self_name} env-si.lnt -s E:/a.c "
		
	end.parse!
rescue
	param_error $!
end

#是否需要重定向到工程目录
is_need_redirect = nil

#保存脚本绝对路径
self_path = File.realpath(__FILE__)
curr_dir = Dir.pwd()
if File.dirname(self_path) == curr_dir
	is_need_redirect = true
end

#工程目录
prj_dir = File.realpath("../../../", self_path)
if is_need_redirect
	Dir.chdir(prj_dir)
end

#将工程目录添加到环境变量
ENV['PROJ_PATH'] = relative_path(prj_dir)

#调用lint
puts "Current Dir=#{Dir.pwd}"
puts "Project Path=#{prj_dir}"
puts "Project Relativate Path=#{ENV['PROJ_PATH']}"
if $options.src_file_path
	puts "lint file"	
	cmd = "Lint-nt +v -i#{prj_dir}/Tools/PC-lint std.lnt -u -os(_LINT.txt) #{ARGV.join(' ')}   #{$options.src_file_path}"
	puts cmd
	system(cmd)
else
	puts "Genarate check file list"
	cmd = "ruby #{prj_dir}/Tools/PC-lint/make_file_list.rb #{prj_dir}/IAR_project/Spump18_slaver.ewp>src_file.lnt"
	system(cmd)
	puts "lint the files"
	cmd = "Lint-nt +v -i#{prj_dir}/Tools/PC-lint std.lnt  -os(_LINT.txt) #{ARGV.join(' ')}   src_file.lnt"
	system(cmd)
end

#如果是重定向的,则再回来
if is_need_redirect
	if curr_dir != Dir.pwd
		#puts "move file"
		FileUtils.mv("_LINT.txt", curr_dir)
		if $options.src_file_path == nil
			FileUtils.mv("src_file.lnt", curr_dir)
		end
		Dir.chdir(curr_dir)
	end
end

#system("type _LINT.txt")
str = File.read "_LINT.txt"
puts "---finish lint---"
puts "output placed in _LINT.txt"
puts "-"*100
puts str
