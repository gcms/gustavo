package ui;

import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FilenameFilter;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

import base.ConnectionFactory;
import base.RequestFactory;
import base.StateFactory;

public class ModuleLoader {
	URLClassLoader cl = new URLClassLoader(new URL[] {});

	public static class Module {
		private String name;

		public String getName() {
			return name;
		}

		private Class requestFactoryClass;

		public RequestFactory getRequestFactory() {
			try {
				return (RequestFactory) requestFactoryClass.newInstance();
			} catch (InstantiationException e) {
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				e.printStackTrace();
			}

			return null;
		}

		private Constructor connectionFactoryClassConstructor;

		public ConnectionFactory getConnectionFactory(URI uri) {
			try {
				return (ConnectionFactory) connectionFactoryClassConstructor
						.newInstance(new Object[] { uri });
			} catch (IllegalArgumentException e) {
				e.printStackTrace();
			} catch (InstantiationException e) {
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				e.printStackTrace();
			}

			return null;
		}

		private Class stateFactoryClass;

		public StateFactory getStateFactory() {
			try {
				return (StateFactory) stateFactoryClass.newInstance();
			} catch (InstantiationException e) {
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				e.printStackTrace();
			}

			return null;
		}
	}

	private Map modules = new HashMap();

	public void loadModule(String name, String connectionFactoryString,
			String stateFactoryString, String requestFactoryString) {
		try {
			Module module = new Module();
			module.name = name;

			Class connectionFactoryClass = Class.forName(
					connectionFactoryString, true, cl);
			module.connectionFactoryClassConstructor = connectionFactoryClass
					.getConstructor(new Class[] { Class.forName("java.net.URI",
							true, cl) });

			module.stateFactoryClass = Class.forName(stateFactoryString, true,
					cl);
			module.requestFactoryClass = Class.forName(requestFactoryString,
					true, cl);

			modules.put(module.name, module);

		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		}
	}

	public void loadPath(String modulePath) {
		URL[] urls = new URL[cl.getURLs().length + 1];
		try {
			urls[0] = new URL(modulePath);
		} catch (MalformedURLException e1) {
			e1.printStackTrace();
			return;
		}
		System.arraycopy(urls, 1, cl.getURLs(), 0, cl.getURLs().length);
		cl = new URLClassLoader(urls);

		File modulePathname = new File(modulePath);

		if (modulePathname.exists() && modulePathname.isDirectory()) {
			File[] moduleFiles = modulePathname.listFiles(new FileFilter() {
				public boolean accept(File arg0) {
					return arg0.exists() && arg0.isDirectory();
				}
			});

			for (int i = 0; i < moduleFiles.length; i++) {
				File m = moduleFiles[i];

				File[] propertiesFile = m.listFiles(new FilenameFilter() {
					public boolean accept(File arg0, String arg1) {
						return arg0.exists() && arg1.equals("properties");
					}
				});

				if (propertiesFile != null && propertiesFile.length > 0) {
					Properties p = new Properties();

					try {
						p.load(new FileInputStream(propertiesFile[0]));
						loadModule(m.getName(), p
								.getProperty("connectionFactory"), p
								.getProperty("stateFactory"), p
								.getProperty("requestFactory"));

					} catch (FileNotFoundException e) {
						e.printStackTrace();
					} catch (IOException e) {
						e.printStackTrace();
					} catch (SecurityException e) {
						e.printStackTrace();
					}

				}
			}
		}
	}

	public Module getModule(String moduleName) {
		return (Module) modules.get(moduleName);
	}

	public String[] getAvailableModulesNames() {
		return (String[]) modules.keySet().toArray(new String[] {});
	}

	public Module[] getAvailableModules() {
		return (Module[]) modules.entrySet().toArray(new Module[] {});
	}
}
