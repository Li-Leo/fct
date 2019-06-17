#!/usr/bin/env ruby
require 'optparse'
require 'ostruct'
#require 'fileutils'
require 'pathname'
require 'rexml/document'
#include REXML

$self_name = File.basename(__FILE__, ".rb")

#������Ϣ
$help_str = %Q{This is used to generate src_file.lnt for PC-lint from IAR project file.
Usage: #{$self_name} [option] [file]	}

#��ӡ���������˳�
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

#��ʼ������option
$options = OpenStruct.new
$options.outfile_name = nil
$options.list_file_name = nil
begin
	#����option,����-h,--help,-v,--version���Դ�֧�ֵ�,���Բ������
	OptionParser.new do |opts|
		#���ó���İ�����Ϣ
		opts.banner = $help_str
		#���ó���İ汾��Ϣ
		opts.version = "1.0.0"
		
		# ��һ����Short option���ڶ�����Long option���������Ƕ�Option������
		opts.on("-o", "--out=out_file",
		"output to this file") do |file_name|
			$options.outfile_name = file_name
		end

		# ��һ����Short option���ڶ�����Long option���������Ƕ�Option������
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



#��IAR�����ļ��е�c�ļ���ȡ������ת��Ϊ����·��
def to_normal_path(file_path)
	#����ļ�����c�ͷ���nil
	if File.extname(file_path).downcase != '.c'
		return nil
	end

	#���ַ�����$word$ת��Ϊ��ͨ�ַ���
	return file_path.gsub(/\$(\w+)\$/) do |match|
		var = $1
		ret = nil
		#�������$PROJ_DIR$��ת��Ϊ.
		if var == 'PROJ_DIR'
			ret = '.'
		elsif var =~ /^_(\w+)_$/
			#�������$_word_$��ת���ڻ�������word��ֵ
			var = $1
			ret = ENV[var]
		else
			raise "unknow file: #{file_path}"
		end	
		ret
	end
end


#����һ���ļ�
def handle_file(infile, infile_name, outfile, outfile_name)
	#��ȡ���̵ľ���·��
	infile_path = File.realpath(infile_name)
	curr_dir = Dir.pwd()

	#����IAR�����ļ�
	xml = REXML::Document.new(infile) 
	#puts xml.root
	#��ȡIAR����������Ϊfile��Ԫ��
	xml.each_element('//file') do |ele|
		if ele.elements['name']
			#fileԪ�ص���Ԫ��name��������ļ�·��
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

#�������ļ��б�������,���ʹ���ն�����,������Ϊ��
#$infile_names = [""]
$infile_names = []
if ARGV.size > 0
	$infile_names = ARGV
end

#��������ļ�
if $options.outfile_name
	$outfile = File.open($options.outfile_name, "w")
	$outfile_name = $options.outfile_name
else
	$outfile = $stdout
	$outfile_name = ""
end

#���δ�������ļ�
for file_name in $infile_names
	if file_name == ""
		infile = $stdin
	else
		infile = File.open file_name
		#$outfile.puts file_name+":"
	end
	#���ν��������ļ�
	handle_file(infile, file_name, $outfile, $outfile_name)
end

#���δ����б��ļ��е��ļ�
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