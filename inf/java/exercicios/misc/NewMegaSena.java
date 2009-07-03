/*
 * NewMegaSena.java
 * Copyright (C) 2004 Gustavo Cipriano Mota Sousa <gustavomota@inf.ufg.br>
 */

package exercicios.misc;

import java.net.URL;
import java.net.JarURLConnection;
import java.net.MalformedURLException;

import java.util.Vector;

import java.util.zip.ZipEntry;

import java.util.regex.Pattern;
import java.util.regex.Matcher;

import java.io.InputStream;
import java.io.IOException;

/**
 * Programa que pega um arquivo contendo valores de jogos da Mega Sena
 * e gera pontos para gerar um grafico com estatisticas sobre os jogos.
 */
public class NewMegaSena {
    /**
     * Retorna uma InputStream para leitura de um arquivo contido em
     * um arquivo compactado da URL especificada como argumento.
     * @param url String contendo url para a entrada em um arquivo jar
     * especificada na descricao da classe JarURLConnection.
     * @return InputStream para leitura dos dados
     */
    public static InputStream getURLInputStream(final String url) {
        try {
            URL site = new URL(url);
            JarURLConnection jarConn = (JarURLConnection) site.openConnection();
            return (jarConn.getJarFile()).
                getInputStream((ZipEntry) jarConn.getJarEntry());
        } catch (MalformedURLException e) {
            System.err.println("ERROR: " + e);
        } catch (IOException e) {
            System.err.println("ERROR: " + e);
        }

        return null;
    }

    /**
     * Le uma linha de uma InputStream.
     * @param is InputStream
     * @return String contendo caracteres da proxima linha de uma InputStream
     */
    public static String getStringFromInputStream(final InputStream is) {
        int read;
        StringBuffer buff = new StringBuffer();

        try {
            while ((read = is.read()) >= 0) {
                if ((char) read == '\n') {
                    break;
                }
                buff.append((char) read);
            }
        } catch (IOException e) {
            System.err.println("ERROR: " + e);
        }
        return new String(buff);
    }

    /**
     * Testa um padrao com uma string e retorna um array
     * contendo as strings que "combinaram" com o padrao.
     * @param pattern padrao POSIX REG_EXTENDED
     * @param str String a ser testada
     * @return array contendo strings combinadas
     */
    public static String[] matches(final String pattern, final String str) {
        Pattern p = Pattern.compile(pattern);
        Matcher m = p.matcher(str);

        if (m.find()) {
            String[] result = new String[m.groupCount()];
            for (int i = 1; i <= m.groupCount(); i++) {
                result[i - 1] = new String(m.group(i));
            }
            return result;
        }
        return null;
    }

    /**
     * Retorna array contendo dados dos jogos da mega sena.
     * @return array contendo dados dos jogos
     */
    private static byte[] arrayMegaSena() {
        final String jarURLStr = "jar:http://www1.caixa.gov.br/loterias/"
                + "_arquivos/loterias/D_megase.zip!/D_MEGA.HTM";
        final String datePatternStr = "<td>[0-9]{2}/[0-9]{2}/[0-9]{4}</td>";
        final String numPatternStr = "<td>([0-9]{2})</td>";

        InputStream is = getURLInputStream(jarURLStr);
        Vector vetor = new Vector();

        while (true) {
            String str = new String(getStringFromInputStream(is));

            if (str.length() <= 0) {
                break;
            }

            /* se encontrar a data */
            String[] matches = matches(datePatternStr, str);

            /* pega os numeros seguintes */
            if (matches != null) {
                while ((matches = matches(numPatternStr,
                            getStringFromInputStream(is))) != null) {
                    vetor.addElement(new Byte(matches[0]));
                }
            }
        }
        byte[] result = new byte[vetor.size()];
        for (int i = 0; i < vetor.size(); i++) {
            result[i] = ((Byte) vetor.elementAt(i)).byteValue();
        }

        return result;
    }

    /**
     * Metodo principal.
     * @param args Argumentos da linha de comando
     */
    public static void main(final String[] args) {
        final byte numDezenas = 6;
        byte[] jogos = arrayMegaSena();

        int soma = 0;
        for (int i = 0; i < jogos.length; i++) {
            soma += jogos[i];
            if ((i + 1) % numDezenas == 0) {
                System.out.println((i / numDezenas + 1) + " "
                        + (soma / numDezenas));
                soma = 0;
            }
        }
    }
}
