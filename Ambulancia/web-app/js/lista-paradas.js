Ext.ns('Ambulancia');


Ambulancia.FieldSet = Ext.extend(Ext.Panel, {
    layout: 'column',
    border: false,
    frame: false,
    bodyStyle: 'padding-top: 10pt',

    initComponent : function() {
        var config = Ext.apply({}, this.initialConfig);
        config = Ext.apply(config, {
            columnWidth: .9,
            border: false,
            width: undefined
        });

        this.fieldSet = new Ext.form.FieldSet(config);

        this.buttonDelete = new Ext.form.FieldSet({
            columnWidth: .1,
            defaultType: 'button',
            border: false,
            items: [
                {
                    width: 20,
                    handler: config.handler.createDelegate(this),
                    icon: '../images/skin/delete.png'
                }
            ]
        });


        this.items = [this.fieldSet, this.buttonDelete];
        Ambulancia.FieldSet.superclass.initComponent.call(this);

        this.changeFieldNames();
    },

    changeFieldNames: function() {
        var fields = this.fieldSet.items.items;

        for (var i = 0; i < fields.length; i++) {
            var field = fields[i];
            var originalName = this.initialConfig.items[i].name;
            var newName = 'paradas[' + this.index + '].' + originalName;

            field.name = newName;
            field.originalName = originalName;

            if (field.getEl())
                field.getEl().dom.name = newName;
        }
    },

    setValues: function(values) {
        var fields = this.fieldSet.items.items;

        for (var i = 0; i < fields.length; i++) {
            var field = fields[i];
            var value = values[field.originalName];

            if (!value)
                continue;

            field.value = value;

            if (field.getEl())
                field.getEl().dom.value = value;
        }
    },

    setIndex: function(index) {
        this.index = index;
        this.changeFieldNames();
    }

});

Ambulancia.FieldSetList = function(config) {
    this.renderTo = config.renderTo;
    this.lista = [];
};

Ambulancia.FieldSetList.prototype = {
    adicione: function (items, values) {
        var obj = this;
        var panel = new Ambulancia.FieldSet({
            defaultType: 'textfield',
            defaults: { width: 160 },
            width: 250,
            labelWidth: 50,
            //        index: 0,
            items: items,
            handler: function() {
                var el = Ext.get(this.id);
                el.remove();

                obj.lista.remove(this);
                obj.atualize();
            }
        });
        panel.render(this.renderTo);

        this.lista.push(panel);
        this.atualize();

        if (values)
            panel.setValues(values);

        return panel;
    },

    atualize: function() {
        for (var i = 0; i < this.lista.length; i++) {
            this.lista[i].setIndex(i);
        }
    }
};

Ambulancia.FieldSetList.TiposParadas = {
    'br.gov.go.saude.hugo.ambulancia.ParadaPaciente': [
        {
            fieldLabel: 'Paciente',
            name: 'paciente',
            allowBlank: true//false
        },
        {
            fieldLabel: 'AIH',
            name: 'aih',
            allowBlank: true//false
        },
        {
            fieldLabel: 'Destino',
            name: 'destino',
            allowBlank: true//false
        },
        {
            xtype: 'hidden',
            name: 'class',
            value: 'br.gov.go.saude.hugo.ambulancia.ParadaPaciente'
        }
    ],
    'br.gov.go.saude.hugo.ambulancia.ParadaServicos': [
        {
            fieldLabel: 'Serviços',
            name: 'descricao',
            allowBlank: true//false
        },
        {
            fieldLabel: 'Destino',
            name: 'destino',
            allowBlank: true//false
        },
        {
            xtype: 'hidden',
            name: 'class',
            value: 'br.gov.go.saude.hugo.ambulancia.ParadaServicos'
        }
    ],

    build: function(obj) {
        var result = {};
        for (var k in values) {
            if (k == 'class') {
                result.push({
                    xtype: 'hidden',
                    name: 'class',
                    value: values[k]
                });
            } else if (k != 'id') {
                result.push({
                    fieldLabel: k.capitalize(),
                    name: k,
                    allowBlank: false,
                    value: values[k]
                });
            }
        }
    }
};