package ui;

import java.io.File;
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

	/* TODO: change exception handling */
	public void loadModule(String name, String connectionFactoryString,
			String stateFactoryString, String requestFactoryString)
			throws InvalidModuleException {

		Module module = new Module();
		module.name = name;

		try {
			Class connectionFactoryClass;
			connectionFactoryClass = Class.forName(connectionFactoryString,
					true, cl);
			module.connectionFactoryClassConstructor = connectionFactoryClass
					.getConstructor(new Class[] { Class.forName("java.net.URI",
							true, cl) });

			module.stateFactoryClass = Class.forName(stateFactoryString, true,
					cl);
			module.requestFactoryClass = Class.forName(requestFactoryString,
					true, cl);

			modules.put(module.name, module);

			System.out.println(module.name + " loaded");

		} catch (ClassNotFoundException e) {
			// e.printStackTrace();
			throw new InvalidModuleException(e);
		} catch (SecurityException e) {
			// e.printStackTrace();
			throw new InvalidModuleException(e);
		} catch (NoSuchMethodException e) {
			// e.printStackTrace();
			throw new InvalidModuleException(e);
		}

	}

	public void loadPath(String urlString) throws InvalidModuleException {
		try {
			loadPath(new URL(urlString));
		} catch (InvalidModuleException e) {
			// e.printStackTrace();
			throw new InvalidModuleException(e);
		} catch (MalformedURLException e) {
			// e.printStackTrace();
			throw new InvalidModuleException(e);
		}
	}

	public void loadPath(File file) throws InvalidModuleException {
		try {
			loadPath(file.toURL());
		} catch (InvalidModuleException e) {
			// e.printStackTrace();
			throw new InvalidModuleException(e);
		} catch (MalformedURLException e) {
			// e.printStackTrace();
			throw new InvalidModuleException(e);
		}
	}

	public void loadPath(URL url) throws InvalidModuleException {
		cl = new URLClassLoader(new URL[] { url }, cl);

		File moduleDirectory = new File(url.getPath());

		if (!moduleDirectory.exists() || !moduleDirectory.isDirectory()) {
			throw new InvalidModuleException("Invalid directory module");
		}

		File[] propertiesFile = moduleDirectory.listFiles(new FilenameFilter() {
			public boolean accept(File arg0, String arg1) {
				return arg0.exists() && arg1.equals("properties");
			}
		});

		if (propertiesFile != null && propertiesFile.length > 0) {
			Properties p = new Properties();

			try {
				p.load(new FileInputStream(propertiesFile[0]));

				loadModule(p.getProperty("moduleName"), p
						.getProperty("connectionFactory"), p
						.getProperty("stateFactory"), p
						.getProperty("requestFactory"));

			} catch (FileNotFoundException e) {
				// e.printStackTrace();
				throw new InvalidModuleException(e);
			} catch (IOException e) {
				// e.printStackTrace();
				throw new InvalidModuleException(e);
			}
		} else {
			throw new InvalidModuleException(
					"properties file not found for directory " + url.getPath());
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