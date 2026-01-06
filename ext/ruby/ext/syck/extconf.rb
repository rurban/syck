[ 'emitter.c', 'gram.c', 'gram.h', 'handler.c', 'node.c', 'syck.c', 'syck.h', 'token.c',
  'bytecode.c', 'bytecode.h', 'implicit.c', 'yaml2byte.c', 'yamlbyte.h' ].each do |codefile|
    `cp #{File::dirname $0}/../../../../lib/#{codefile} #{File::dirname $0}/#{codefile}`
end

require 'mkmf'

have_header( "ruby/st.h" )
dir_config( "syck" )
create_makefile( "syck" )
