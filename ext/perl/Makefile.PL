use 5.006;
use strict;
use warnings;
use ExtUtils::MakeMaker;

require "./inc/ExtUtils/HasCompiler.pm";

my $can_xs
    = ExtUtils::HasCompiler::can_compile_loadable_object( quiet => 1 )
    ? 1
    : 0;
if ( !$can_xs ) {
    print
        "Sorry! YAML::Syck requires the C compiler which built your perl in order to be built.\n";
    exit 1;
}

my $bad;
if ( eval { require YAML; $YAML::VERSION < 0.60 } ) {
    print "*** Pre-0.60 version of YAML.pm ($YAML::VERSION) detected.\n";
    $bad++;
}
if ( eval { require YAML::Syck; $YAML::Syck::VERSION < 0.60 } ) {
    print
        "*** Pre-0.60 version of YAML::Syck ($YAML::Syck::VERSION) detected.\n";
    $bad++;
}

if ($bad) {
    print <<'_EOF';

*** WARNING ***

YAML::Syck version >=0.60 breaks compatibility with earlier versions of
YAML::Syck and YAML.pm (<0.60) when serializing blessed references.

See the COMPATIBILITY file for more information.

_EOF

    my $ans = prompt( "Continue installing YAML::Syck?", "y" );
    exit() unless $ans =~ /^y/i;
}

sub copy_with_coda {
    my ( $from_file, $to_file ) = @_;
    open my $from, '<', $from_file
        or die "Could not read $from_file: $!";
    chmod 0666, $to_file if -e $to_file;
    open my $out, '>>', $to_file
        or die "Could not write $to_file: $!";
    print $out
        "/* ex: set ro ft=c: -*- mode: c; buffer-read-only: t -*- */\n";
    print $out "/* Auto-copied from syck/lib source tree */\n";
    print {$out} do { local $/; <$from> };
    close $from;
    close $out;
    chmod 0444, $to_file;
}

my $DEFINE;
my @SYCK_FILES
    = qw(bytecode.c bytecode.h emitter.c gram.c gram.h handler.c implicit.c
    node.c syck.c syck_st.c token.c
    sycklex.h syck.h syck_st.h);

# are we in ext/syck/ or a standalone cpan distribution?
if ( -f "../../lib/syck.c" ) {
    for my $c (@SYCK_FILES) {
        my $to = $c eq "syck.c" ? "syck_lib.c" : $c;
        copy_with_coda( "../../lib/$c", $to );
    }
    copy_with_coda( "../../config.h", "config.h" );
    $DEFINE = "-DHAVE_CONFIG_H";
}

# One liner stolen from inc/Module/Install/Compiler.pm
my $c_files = join ' ',
    map { substr( $_, 0, -2 ) . $Config::Config{_o} }
    ( glob("*.c"), ( -e 'Syck.c' ? () : 'Syck.c' ) );

WriteMakefile(
    NAME               => 'YAML::Syck',
    AUTHOR             => q{Todd Rinaldo <toddr@cpan.org>},
    VERSION_FROM       => 'lib/YAML/Syck.pm',
    ABSTRACT_FROM      => 'lib/YAML/Syck.pm',
    LICENSE            => 'BSD',
    PL_FILES           => {},
    MIN_PERL_VERSION   => '5.006',
    CONFIGURE_REQUIRES => { 'ExtUtils::MakeMaker' => '0', },
    BUILD_REQUIRES     => {
        'Test::More' => '0',
        defined $ENV{'AUTOMATED_TESTING'} ? ( 'Devel::Leak' => 0 ) : (),

    },
    $DEFINE ? ( DEFINE => $DEFINE ) : (),
    INC    => "-I.",
    OBJECT => $c_files,
    dist   => { COMPRESS => 'gzip -9f', SUFFIX => 'gz', },
    clean  => { FILES => "syck_lib.c config.h " . join( " ", @SYCK_FILES ) },
    $ExtUtils::MakeMaker::VERSION >= 6.46
    ? ( 'META_MERGE' => {
            resources => {
                'license'    => 'http://dev.perl.org/licenses/',
                'homepage'   => 'http://github.com/toddr/YAML-Syck',
                'bugtracker' => 'https://github.com/toddr/YAML-Syck/issues',
                'repository' => 'http://github.com/toddr/YAML-Syck',
            }
        }
        )
    : (),
);

package MY;

sub depend {
    "
release : dist
	git tag v\$(VERSION)
	cpan-upload \$(DISTVNAME).tar\$(SUFFIX)
	git push
	git push --tags
"
}
