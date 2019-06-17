#!/usr/bin/env ruby
require 'optparse'
require 'ostruct'
#require 'fileutils'
require 'pathname'
require 'rexml/document'
#include REXML

$self_name = File.basename(__FILE__, ".rb")

#帮助信息
$help_str = %Q{This is used to generate src_file.lnt for PC-lint from IAR project file.
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

#初始化所有option
$options = OpenStruct.new
$options.outfile_name = nil
$options.list_file_name = nil
begin
	#解析option,其中-h,--help,-v,--version是自带支持的,可以不用添加
	OptionParser.new do |opts|
		#设置程序的帮助信息
		opts.banner = $help_str
		#设置程序的版本信息
		opts.version = "1.0.0"
		
		# 第一项是Short option，第二项是Long option，第三项是对Option的描述
		opts.on("-o", "--out=out_file",
		"output to this file") do |file_name|
			$options.outfile_name = file_name
		end

		# 第一项是Short option，第二项是Long option，第三项是对Option的描述
		opts.on("-l", "--list_file=list_file",
		"the list file include the source files, each line is a source file") do |list_file_name|
			$options.list_file_name = list_file_name
		end

		opts.separator "Example:"
		opts.separator "#{$self_name} a.c b.c "
		
	end.parse!
rescue
	param_error $!
end



#将IAR工程文件中的c文件提取出来并转换为正常路径
def to_normal_path(file_path)
	#如果文件后不是c就返回nil
	if File.extname(file_path).downcase != '.c'
		return nil
	end

	#将字符串中$word$转化为普通字符串
	return file_path.gsub(/\$(\w+)\$/) do |match|
		var = $1
		ret = nil
		#如果碰到$PROJ_DIR$就转换为.
		if var == 'PROJ_DIR'
			ret = '.'
		elsif var =~ /^_(\w+)_$/
			#如果碰到$_word_$就转化在环境变量word的值
			var = $1
			ret = ENV[var]
		else
			raise "unknow file: #{file_path}"
		end	
		ret
	end
end


#处理一个文件
def handle_file(infile, infile_name, outfile, outfile_name)
	#获取工程的绝对路径
	infile_path = File.realpath(infile_name)
	curr_dir = Dir.pwd()

	#解析IAR工程文件
	xml = REXML::Document.new(infile) 
	#puts xml.root
	#获取IAR中所有名字为file的元素
	xml.each_element('//file') do |ele|
		if ele.elements['name']
			#file元素的子元素name里包含了文件路径
			file_path =  to_normal_path(ele.elements['name'].text)
			if file_path && file_path[0] == '.'
				#puts infile_path,file_path
				file_real_path = File.realpath(file_path, File.dirname(infile_path))
				file_path = Pathname(file_real_path).relative_path_from(Pathname(curr_dir)).to_s
				#file_path = File.relative_path_from(curr_dir, file_real_path)
				#puts file_path
			end
			if file_path
				outfile.puts file_path
			end
		end
	end
end

#main
if ARGV.size == 0 && !$options.list_file_name
	param_error("no input file")
end

#将输入文件列表保存起来,如果使用终端输入,则名字为空
#$infile_names = [""]
$infile_names = []
if ARGV.size > 0
	$infile_names = ARGV
end

#设置输出文件
if $options.outfile_name
	$outfile = File.open($options.outfile_name, "w")
	$outfile_name = $options.outfile_name
else
	$outfile = $stdout
	$outfile_name = ""
end

#依次处理各个文件
for file_name in $infile_names
	if file_name == ""
		infile = $stdin
	else
		infile = File.open file_name
		#$outfile.puts file_name+":"
	end
	#依次解析各个文件
	handle_file(infile, file_name, $outfile, $outfile_name)
end

#依次处理列表文件中的文件
if $options.list_file_name
	infile = File.open $options.list_file_name
	infile.each_line do |line|
		line.chomp!
		if line != ""
			infile = File.open line
			handle_file(infile, line, $outfile, $outfile_name)
		end
	end
end