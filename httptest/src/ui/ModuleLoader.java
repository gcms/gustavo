package ui;

import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FilenameFilter;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.net.URI;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

import base.ConnectionFactory;
import base.RequestFactory;
import base.StateFactory;

public class ModuleLoader {
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

		private Class connectionFactoryClass;

		public ConnectionFactory getConnectionFactory(URI uri) {
			Constructor c;
			try {
				c = connectionFactoryClass.getConstructor(new Class[] { Class
						.forName("java.net.URI") });
				return (ConnectionFactory) c.newInstance(new Object[] { uri });
			} catch (SecurityException e) {
				e.printStackTrace();
			} catch (NoSuchMethodException e) {
				e.printStackTrace();
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
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

	private Map modules;

	public ModuleLoader(String modulePath) {
		modules = new HashMap();

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
						Module module = new Module();
						module.name = m.getName();

						p.load(new FileInputStream(propertiesFile[0]));

						module.connectionFactoryClass = Class.forName(p
								.getProperty("connectionFactory"));
						module.stateFactoryClass = Class.forName(p
								.getProperty("stateFactory"));
						module.requestFactoryClass = Class.forName(p
								.getProperty("requestFactory"));

						modules.put(module.name, module);

					} catch (FileNotFoundException e) {
						e.printStackTrace();
					} catch (IOException e) {
						e.printStackTrace();
					} catch (ClassNotFoundException e) {
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
