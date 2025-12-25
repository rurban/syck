[ 'emitter.c', 'gram.c', 'gram.h', 'handler.c', 'node.c', 'syck.c', 'syck.h', 'syck_st.h', 'syck_st.c', 'token.c', 'bytecode.c', 'implicit.c', 'yaml2byte.c', 'yamlbyte.h' ].each do |codefile|
    `cp #{File::dirname $0}/../../../../lib/#{codefile} #{File::dirname $0}/#{codefile}`
end

require 'mkmf'

#have_header( "st.h" )
$CPPFLAGS << "-DIN_OLD_EXT_RUBY"
dir_config( "syck" )
create_makefile( "syck" )
