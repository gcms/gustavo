package gustavocms.translator

/**
 * Created by IntelliJ IDEA.
 * User: fibo
 * Date: Feb 9, 2010
 * Time: 5:30:34 PM
 * To change this template use File | Settings | File Templates.
 */
public class Babylon {
    private static final int MAX_WORD_LENGTH = 63
    
    RandomAccessFile fileIdx // file_def
    RandomAccessFile fileDef // file_idx

    public Babylon(File index, File definition) {
        fileIdx = new RandomAccessFile(index, "r")
        fileDef = new RandomAccessFile(definition, "r")
    }

    // sem espaćos, outros símbolos, e em minúscula

    public List translate(String word) {
        if (!isWord(word))
            throw new IllegalArgumentException("Palavra $word contém caracteres inválidos")

        if (word.length() >= MAX_WORD_LENGTH)
            return []

        doTranslate(fillSmallWord(word))
    }

    private boolean isWord(String word) {
        (word.chars as List).every {Character ch -> ch.isLetter() }
    }

    private String fillSmallWord(String word) {
        int numChars = Math.max(3 - word.length(), 0)
        word + ("_" * numChars)
    }

    private List doTranslate(String word) {
        long mainIdx = 100 + 4 * wordIndex(word)
        printf("main_index = %d\n", mainIdx)

        fileIdx.seek(mainIdx)
        fileDef.seek(mainIdx)

        long idxIdx = readVal(fileIdx, 4)
        long idxDef = readVal(fileDef, 4)

        printf("idx: %d, def: %d\n", idxIdx, idxDef)

        if (idxDef == readVal(fileDef, 4))
            return []

        printf("idx_def != read_val(file_def, 4)\n")

        fileIdx.seek(idxIdx)


        long minSize = readVal(fileIdx, 1)
        long maxSize = readVal(fileIdx, 1)

        printf("min: %d, max: %d\n", minSize, maxSize)

        if (word.length() < minSize || word.length() > maxSize)
            return []

        long wc = 0
        int byteToSkip = 0

        long nw
        for (long i = minSize; i < word.length(); i++) {
            nw = readVal(fileIdx, 2)
            wc += nw
            byteToSkip += (i - 3) * nw
            printf("\twc: %d, nw: %d\n", wc, nw)
        }
        nw = readVal(fileIdx, 2)
        byteToSkip += (maxSize - word.size()) * 2

        printf("wc: %d, nw: %d\n", wc, nw)

        fileIdx.skipBytes(byteToSkip)

        List result = []
        for (int i = 0; i < nw; i++) {
            int j = 3
            while (j < word.length() && word.charAt(j) == fileIdx.read())
                j++

            if (j == word.length()) {
                long val
                fileDef.seek(idxDef + 4 * wc)
                val = readVal(fileDef, 4)

                if ((val & 0xFF000000) != 0) {
                    mainIdx = 100 + 4 * ((val >> 16) & 0xFFFF)
                    fileDef.seek(mainIdx)
                    idxDef = readVal(fileDef, 4)

                    fileDef.seek(idxDef + 4 * (val & 0xffff))
                    val = readVal(fileDef, 4)
                }

                fileDef.seek(val)
                result.add(makeDefinition(val))
            } else {
                fileIdx.skipBytes(word.length() - j - 1)
            }
            ++wc

        }

        result
    }

    private static char getAttribute(int pos) {
        "0123456789ABCDEF".charAt(pos)
    }


    private  Item makeDefinition(long pos) {
        fileDef.seek(pos)

        printf("pos = %d, tell = %d\n", pos, fileDef.filePointer)

        readDefinition(fileDef)
    }

    private static Item readDefinition(DataInput fileDef) {
        StringBuilder attrib = new StringBuilder()
        7.times {
            short val = readVal(fileDef, 1)
            attrib.append(getAttribute((int) (val/16)))
            attrib.append(getAttribute((int) (val%16)))
        }
        println "attrib: " + attrib

        int sz = readVal(fileDef, 1)
        printf("makeDef: sz = %d\n", sz)

        String word = readWord(fileDef, sz)
        printf("read_word(%d) = %s\n", sz, word)

        sz = readVal(fileDef, 1)

        int mask = 0
        int c = 0, prevC
        StringBuilder definition = new StringBuilder()

        while (sz > 0) {
            prevC = c
            c = readVal(fileDef, 1) ^ mask
            mask ^= 0x80
            if (c == 0)
                continue

            if (prevC == 0x7D) {
                if (c == 0xEF) {
                    definition.append ', '
                    sz -= 3
                } else {
                    definition.append((char) prevC)
                    definition.append((char) c)
                    sz -= 2
                }
            }
            else if (prevC == 0x7F) {
                if (c == 0xEF) {
                    definition.append('. ')
                    sz -= 3 // Considered as 3 chars.. Hum..
                }
            } else if (prevC == 0xFF) {
                if (c == 0xF3) { // [FF F3]
                    definition.append('..;')
                } else if (c == 0xEF) { // [FF EF]
                    definition.append('.. ')
                } else if (c == 0xFF) { // [FF FF]
                    definition.append('...')
                }
                sz -= 3
                c = 0 // 'clear previous char'
            } else if (prevC == 0xFB) {
                if (c == 0xFF) {
                    definition.append('..')
                    sz -= 3 // COnsidered as 3 chars.. Hum..
                    c = 0 // 'clear previous char'
                } else {
                    definition.append((char) prevC)
                    definition.append((char) c)
                    sz -= 2
                }
            } else if ( c == 0xFB || c == 0xFF || c == 0x7D || c == 0x7F) {
                ; /* wait */
            } else {
                definition.append((char) c)
                sz--
            }
        }

        new Item(word: word, definition: definition, attributes: attrib)
    }

    private static String read2ByteSequence(int word) {
        StringBuilder sb = new StringBuilder()

        if ((word & 0x8000) == 0) {
            3.times {
                sb.append(babToChar(word & 0x1F))
                word = (word >> 5)
            }
        }

        sb.toString()
    }


    private static String readWord(DataInput input, int length) {
        String[] compactTable = ["<0>", "ion", "ies", "ing", "ous", "ses",        /*  0 ..  5 */
                "al", "an",                                          /*  6 ..  7 */
                "at", "ed", "en", "er", "es", "ia", "ic", "ie", /*  8 .. 15 */
                "in", "io", "is", "it", "le", "ly", "ne", "on", /* 16 .. 23 */
                "or", "ra", "se", "ss", "st", "te", "ti", "th"  /* 24 .. 31 */
        ] as String[]

        StringBuilder sb = new StringBuilder()

        int data
        while (length > 0) {
            data = readVal(input, 2)
            if (data < 32768) {                
                sb.append(babToChar(data & 0x1F))
                data = (data >> 5)
                sb.append(babToChar(data & 0x1F))
                data = (data >> 5)
                sb.append(babToChar(data & 0x1F))
                length -= 3
            } else {
                int lsb = data & 0x7F
                int msb = (data >> 8) & 0x7F

                if (lsb >= 32) {
                    sb.append((char) lsb)
                    --length
                } else if (lsb >= 6) {
                    sb.append(compactTable[lsb])
                    length -= 2
                } else {
                    sb.append(compactTable[lsb])
                    length -= 3
                }

                if (length == 0)
                    break

                if (length < 0)
                    return sb.toString()

                if (msb >= 32) {
                    sb.append((char) msb)
                    --length
                } else if (msb >= 6) {
                    sb.append(compactTable[msb])
                    length -= 2
                } else {
                    sb.append(compactTable[msb])
                    length -= 3
                }
            }
        }
        sb.toString()
    }

    private static char babToChar(int pos) {
        "abcdefghijklmnopqrstuvwxyz* **'*".charAt(pos)
    }

    private static long readVal(DataInput input, int n) {
        Utils.readValue(input, n)
    }

    private static long wordIndex(String word) {
        Utils.wordIndex(word)
    }
}
