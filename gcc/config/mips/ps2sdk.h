#undef  LIB_SPEC
#define LIB_SPEC " %{newlib-nano:-lm_nano} %{!newlib-nano:-lm} \
    --start-group \
    %{newlib-nano:%{g:-lg_nano} %{!g:-lc_nano}} %{!newlib-nano:%{g:-lg} %{!g:-lc}}  \
    -lps2sdkc \
    %{kernel-nopatch:-lkernel-nopatch} %{!kernel-nopatch:-lkernel} \
    --end-group"

#undef STARTFILE_SPEC
#define STARTFILE_SPEC "crt0.o%s crti.o%s crtbegin.o%s"

#undef ENDFILE_SPEC
#define ENDFILE_SPEC "crtend.o%s crtn.o%s"
